#include <Objects/Pad.h>

Pad::Pad(SDL_Renderer* renderer, SDL_Rect activeArea, SDL_Rect rect, SDL_Color color) :
    Object(renderer, activeArea, rect, color) { }

Pad::~Pad() { 
    ptrRenderer = nullptr;
}

void Pad::setSpeed(float speed) {
    this->speed = speed;
}

int Pad::collideWall() {
    if(rect.y <= activeArea.y) {
        rect.y = activeArea.y;
        return 1;   // Colisão superior
    }
    else if((rect.y + rect.h) >= activeArea.y + activeArea.h) {
        rect.y = ((activeArea.y + activeArea.h) - rect.h);
        return 2;   // Colisão inferior
    }
    else return 0;  // Sem colisões
}

void Pad::move(bool up) {
    rect.y += up ? (-1 * speed) : speed;
    collideWall();
}