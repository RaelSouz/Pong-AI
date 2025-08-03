#include <Objects/Pad.h>

Pad::Pad(SDL_Renderer* renderer, SDL_Rect activeArea, SDL_Rect rect, SDL_Color color) :
    Object(renderer, activeArea, rect, color) { }

Pad::~Pad() { 
    ptrRenderer = nullptr;
}

void Pad::move(bool up) {
    rect.y += up ? (-1 * speed) : speed;
    if(rect.y <= activeArea.y) {
        rect.y = activeArea.y;  // Colisão superior
    }
    else if((rect.y + rect.h) >= activeArea.y + activeArea.h) {
        rect.y = ((activeArea.y + activeArea.h) - rect.h);   // Colisão inferior
    }
}