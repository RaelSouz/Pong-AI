#include <Objects/Pad.h>

Pad::Pad(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color) :
    Object(window, renderer, rect, color) { }

Pad::~Pad() { 
    ptrRenderer = nullptr;
}

void Pad::setSpeed(float speed) {
    this->speed = speed;
}

int Pad::collideWall() {
    if(rect.y <= 0) {
        rect.y = 0;
        return 1;   // Colisão superior
    }
    else if((rect.y + rect.h) >= win_h) {
        rect.y = (win_h - rect.h);
        return 2;   // Colisão inferior
    }
    else return 0;  // Sem colisões
}

void Pad::move(bool up) {
    rect.y += up ? (-1 * speed) : speed;
    collideWall();
}