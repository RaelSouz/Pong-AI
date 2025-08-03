#include <Objects/Ball.h>

bool isInsideRect(float point_x, float point_y, SDL_Rect* rect);  // Função auxiliar

Ball::Ball(SDL_Renderer* renderer, SDL_Rect activeArea, SDL_Rect rect, SDL_Color color) :
    Object(renderer, activeArea, rect, color), gen(std::random_device{}()) {
    dist = std::uniform_int_distribution<int>(activeArea.h * 0.1, activeArea.h * 0.9);
    randomPos();
}

Ball::~Ball() {
    ptrRenderer = nullptr;
}

void Ball::randomPos() {
    rect.y = dist(gen);
    rect.x = (activeArea.w / 2) - (rect.w / 2); 
    speedX *= (dist(gen) % 2) ? -1 : 1;
    speedY *= (dist(gen) % 2) ? -1 : 1;
}

int Ball::move(Pad* padL, Pad* padR) {
    int outherReturn;
    if((outherReturn = collisionWall()) != 0) return outherReturn; 
    colisionPad(padL);
    colisionPad(padR);

    rect.x += speedX;
    rect.y += speedY;
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

// Colisão em relação ao Pad
void Ball::colisionPad(Pad* pad) {
    SDL_Rect rect2 = pad->getRect();
    if(
        isInsideRect((rect.x + rect.w), rect.y, &rect2)  // Ponto 2
        && isInsideRect((rect.x + rect.w), (rect.y + rect.w), &rect2)  // Ponto 4
    ) { // Colisão na lateral esquerda
        rect.x = rect2.x - rect.w;
        speedX *= -1;
    } else if(
        isInsideRect(rect.x, rect.y, &rect2)  // Ponto 1
        && isInsideRect(rect.x, (rect.y + rect.h), &rect2)  // Ponto 3
    ) { // Colisão na lateral direita
        rect.x = rect2.x + rect2.w;
        speedX *= -1;
    } else if (
        isInsideRect(rect.x, (rect.y + rect.h), &rect2)  // Ponto 3
        || isInsideRect((rect.x + rect.w), (rect.y + rect.w), &rect2)  // Ponto 4
    ) { // Colisão na parte superior
        rect.y = rect2.y - rect.h;
        speedY *= -1;
    } else if (
        isInsideRect(rect.x, rect.y, &rect2)  // Ponto 1
        || isInsideRect((rect.x + rect.w), rect.y, &rect2)  // Ponto 2
    ) { // Colisão na parte inferiror
        rect.y = rect2.y + rect2.h;
        speedY *= -1;
    }
}

bool isInsideRect(float point_x, float point_y, SDL_Rect* rect) {
    return (point_x > rect->x && point_x < (rect->x + rect->w)) 
        && (point_y > rect->y && point_y < (rect->y + rect->h));
}