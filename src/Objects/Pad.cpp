#include <Objects/Pad.h>

Pad::Pad(SDL_FRect activeArea, SDL_FRect rect, SDL_Color color) :
    Object(activeArea, rect, color) { }

Pad::~Pad() {}

void Pad::move(float deltaTime, bool up) {
    rect.y += up ? (-1 * (speed * deltaTime)) : (speed * deltaTime);
    if(rect.y <= activeArea.y) {
        rect.y = activeArea.y;  // Colisão superior
    }
    else if((rect.y + rect.h) >= activeArea.y + activeArea.h) {
        rect.y = ((activeArea.y + activeArea.h) - rect.h);   // Colisão inferior
    }
}