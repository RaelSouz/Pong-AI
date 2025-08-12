#include <Objects/Ball.h>

Ball::Ball(SDL_FRect activeArea, SDL_FRect rect, SDL_Color color) :
    Object(activeArea, rect, color), gen(std::random_device{}()),
    collidedX(false), collidedY(false),
    dist(std::uniform_int_distribution<int>(activeArea.h * 0.1f, activeArea.h * 0.9f)) {
    reset();
}

Ball::~Ball() {}

void Ball::reset() {
    rect.y = dist(gen);
    rect.x = (activeArea.w / 2) - (rect.w / 2); 
    speedX *= (dist(gen) % 2) ? -1.0f : 1.0f;
    speedY *= (dist(gen) % 2) ? -1.0f : 1.0f;
}

int Ball::move(SDL_FRect& padL, SDL_FRect& padR) {
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

void Ball::colisionPad(SDL_FRect& pad) {
    if (SDL_HasIntersectionF(&rect, &pad)) {
        float ballCenterX = rect.x + rect.w / 2;
        float ballCenterY = rect.y + rect.h / 2;
        float padCenterX  = pad.x + pad.w / 2;
        float padCenterY  = pad.y + pad.h / 2;

        float deltaX = ballCenterX - padCenterX;
        float deltaY = ballCenterY - padCenterY;

        float intersectX = (rect.w + pad.w) / 2 - abs(deltaX);
        float intersectY = (rect.h + pad.h) / 2 - abs(deltaY);

        const float PUSH_EPS = 1.0f;

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