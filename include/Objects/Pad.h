#pragma once

#include <Objects/Object.h>

class Pad : public Object {
    public:
        Pad(SDL_FRect activeArea, SDL_FRect rect, SDL_Color color);
        ~Pad();
        
        void move(bool up);

};