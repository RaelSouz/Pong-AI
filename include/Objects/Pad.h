#pragma once

#include <Objects/Object.h>

class Pad : public Object {
    public:
        Pad(SDL_Renderer* renderer, SDL_Rect activeArea, SDL_Rect rect, SDL_Color color);
        ~Pad();
        
        void setSpeed(float speed);
        int collideWall();
        void move(bool up);

        private:
            float speed;

};