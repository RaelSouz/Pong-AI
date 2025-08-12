#include <Objects/Ball.h>

Ball::Ball(SDL_FRect activeArea, SDL_FRect rect, SDL_Color color) :
    Object(activeArea, rect, color), gen(std::random_device{}()),
    collidedX(false), collidedY(false),
    dist(std::uniform_int_distribution<int>(activeArea.h * 0.1f, activeArea.h * 0.9f)) { }

Ball::~Ball() {}

void Ball::reset() {
    rect.y = dist(gen);
    rect.x = (activeArea.w / 2) - (rect.w / 2); 
    speedX *= (dist(gen) % 2) ? -1.0f : 1.0f;
    speedY *= (dist(gen) % 2) ? -1.0f : 1.0f;
}

int Ball::move(float deltaTime, SDL_FRect& padL, SDL_FRect& padR) {
    rect.x += speedX * deltaTime;
    rect.y += speedY * deltaTime;

    // Colisão superior ou inferior
    if(rect.y <= activeArea.y) {    
        rect.y = activeArea.y;
        speedY = -speedY;
    }
    else if((rect.y + rect.h) >= (activeArea.y + activeArea.h)) {
        rect.y = (activeArea.y + activeArea.h) - rect.h;
        speedY = -speedY;
    }
    // Colisão lateral esquerda
    if((rect.x + rect.w) <= activeArea.x) {   
        return 1;
    }
    // Colisão lateral direita
    if(rect.x >= (activeArea.x + activeArea.w)) {
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
    if (!SDL_HasIntersectionF(&rect, &pad)) {
        collidedX = false;
        collidedY = false;
        return;
    }

    float ballCenterX = rect.x + rect.w / 2.0f;
    float ballCenterY = rect.y + rect.h / 2.0f;
    float padCenterX  = pad.x + pad.w / 2.0f;
    float padCenterY  = pad.y + pad.h / 2.0f;

    float deltaX = ballCenterX - padCenterX;
    float deltaY = ballCenterY - padCenterY;

    float intersectX = (rect.w + pad.w) / 2.0f - fabs(deltaX);
    float intersectY = (rect.h + pad.h) / 2.0f - fabs(deltaY);

    if (intersectX < intersectY && !collidedX) {
        rect.x += (deltaX > 0 ? intersectX : -intersectX);
        speedX = -speedX;
        collidedX = true;
        collidedY = false;
    } 
    else if (!collidedY) {
        rect.y += (deltaY > 0 ? intersectY : -intersectY);
        speedY = -speedY;
        collidedY = true;
        collidedX = false;
    }
}