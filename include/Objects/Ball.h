#pragma once

#include <Objects/Object.h>
#include <random>

class Ball : public Object {
    public:
        Ball(SDL_FRect activeArea, SDL_FRect rect, SDL_Color color);
        ~Ball();


        int move(SDL_FRect& padL, SDL_FRect& padR);
        void setSpeed(int speed);
        void reset();

    private:
        std::mt19937 gen;
        std::uniform_int_distribution<int> dist;
        float speedX, speedY;
        bool collidedX, collidedY;

        void colisionPad(SDL_FRect& pad);
};