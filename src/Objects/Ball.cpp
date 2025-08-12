#include <Objects/Ball.h>
#include <iostream>

Ball::Ball(SDL_Rect activeArea, SDL_Rect rect, SDL_Color color) :
    Object(activeArea, rect, color), gen(std::random_device{}()),
    collidedX(false), collidedY(false),
    dist(std::uniform_int_distribution<int>(activeArea.h * 0.1, activeArea.h * 0.9)) {
    reset();
}

Ball::~Ball() {}

void Ball::reset() {
    rect.y = dist(gen);
    rect.x = (activeArea.w / 2) - (rect.w / 2); 
    speedX *= (dist(gen) % 2) ? -1 : 1;
    speedY *= (dist(gen) % 2) ? -1 : 1;
}

int Ball::move(SDL_Rect& padL, SDL_Rect& padR) {
    rect.x += speedX;
    rect.y += speedY;

    if(rect.y <= activeArea.y) {    // Colisão superior
        rect.y = activeArea.y;
        speedY *= -1;
    }
    else if((rect.y + rect.h) >= (activeArea.y + activeArea.h)) {   // Colisão inferior
        rect.y = (activeArea.y + activeArea.h) - rect.h;
        speedY *= -1;
    }
    else if((rect.x + rect.w) <= activeArea.x) {   // Colisão lateral esquerda
        reset(); 
        return 1;
    }
    else if(rect.x >= (activeArea.x + activeArea.w)) {  // Colisão lateral direita
        reset();
        return 2;
    }

    colisionPad(padL);
    colisionPad(padR);

    return 0;
}

void Ball::setSpeed(int speed) {
    this->speed = speed;
    speedX = speed;
    speedY = speed;
}

void Ball::colisionPad(SDL_Rect& pad) {
    if (SDL_HasIntersection(&rect, &pad)) {
        int ballCenterX = rect.x + rect.w / 2;
        int ballCenterY = rect.y + rect.h / 2;
        int padCenterX  = pad.x + pad.w / 2;
        int padCenterY  = pad.y + pad.h / 2;

        int deltaX = ballCenterX - padCenterX;
        int deltaY = ballCenterY - padCenterY;

        int intersectX = (rect.w + pad.w) / 2 - abs(deltaX);
        int intersectY = (rect.h + pad.h) / 2 - abs(deltaY);

        const int PUSH_EPS = 1;

        if (intersectX < intersectY && !collidedX) {
            if (deltaX > 0)
                rect.x = pad.x + pad.w + PUSH_EPS;
            else
                rect.x = pad.x - rect.w - PUSH_EPS;

            speedX = -speedX;
            collidedX = true;
            collidedY = false;
        } else {
            if (!collidedY) {
                if (deltaY > 0)
                    rect.y = pad.y + pad.h + PUSH_EPS;
                else
                    rect.y = pad.y - rect.h - PUSH_EPS;

                speedY = -speedY;
                collidedY = true;
                collidedX = false;
            }
        }
    } else {
        collidedX = false;
        collidedY = false;
    }

}