#pragma once

#include <Objects/Object.h>

class Pad : public Object {
    public:
        Pad(SDL_Rect activeArea, SDL_Rect rect, SDL_Color color);
        ~Pad();
        
        void move(bool up);

};