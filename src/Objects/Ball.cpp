#include <Objects/Ball.h>
#include <iostream>

Ball::Ball(SDL_Rect activeArea, SDL_Rect rect, SDL_Color color) :
    Object(activeArea, rect, color), gen(std::random_device{}()) {
    dist = std::uniform_int_distribution<int>(activeArea.h * 0.1, activeArea.h * 0.9);
    randomPos();
}

Ball::~Ball() {}

void Ball::randomPos() {
    rect.y = dist(gen);
    rect.x = (activeArea.w / 2) - (rect.w / 2); 
    speedX *= (dist(gen) % 2) ? -1 : 1;
    speedY *= (dist(gen) % 2) ? -1 : 1;
}

int Ball::move(Pad* padL, Pad* padR) {
    int outherReturn;

    rect.x += speedX;
    rect.y += speedY;

    if((outherReturn = collisionWall()) != 0) return outherReturn; 
    colisionPad(padL);
    colisionPad(padR);

    return 0;
}

void Ball::setSpeed(int speed) {
    this->speed = speed;
    speedX = speed;
    speedY = speed;
}

int Ball::collisionWall() {
    if(rect.y <= activeArea.y) {    // Colisão superior
        rect.y = activeArea.y;
        speedY *= -1;
    }
    else if((rect.y + rect.h) >= (activeArea.y + activeArea.h)) {   // Colisão inferior
        rect.y = (activeArea.y + activeArea.h) - rect.h;
        speedY *= -1;
    }
    else if((rect.x + rect.w) <= activeArea.x) {   // Colisão lateral esquerda
        randomPos(); 
        return 1;
    }
    else if(rect.x >= (activeArea.x + activeArea.w)) {  // Colisão lateral direita
        randomPos();
        return 2;
    }
    return 0;
}

void Ball::colisionPad(Pad* pad) {
    SDL_Rect rect2 = pad->getRect();
    if(SDL_HasIntersection(&rect, &rect2)) {
        int deltaX = (rect.x + rect.w / 2) - (rect2.x + rect2.w / 2);   // Ball.Center.X - Pad.Center.X
        int deltaY = (rect.y + rect.h / 2) - (rect2.y + rect2.h / 2);   // Ball.Center.Y - Pad.Center.Y
        if (((rect.w + rect2.w) / 2 - abs(deltaX)) < ((rect.h + rect2.h) / 2 - abs(deltaY))) { // intersectX < intersectY -> Lateral
            rect.x = deltaX > 0 ? rect2.x + rect2.w : rect2.x - rect.w; 
            speedX *= -1;
        } else { // Superior ou Inferior
            rect.y = deltaY > 0 ? rect2.y + rect2.h : rect2.y - rect.h; 
            speedY *= -1; 
        }
    }
}