#pragma once

#include <Objects/Object.h>
#include <Objects/Pad.h>
#include <random>

class Ball : public Object {
    public:
        Ball(SDL_Rect activeArea, SDL_Rect rect, SDL_Color color);
        ~Ball();


        int move(SDL_Rect& padL, SDL_Rect& padR);
        void setSpeed(int speed);
        void reset();

    private:
        std::mt19937 gen;
        std::uniform_int_distribution<int> dist;
        int speedX, speedY;
        bool collidedX, collidedY;

        void colisionPad(SDL_Rect& pad);
};