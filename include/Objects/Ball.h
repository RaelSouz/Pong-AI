#pragma once

#include <Objects/Object.h>
#include <Objects/Pad.h>
#include <random>

class Ball : public Object {
    public:
        Ball(SDL_Renderer* renderer, SDL_Rect activeArea, SDL_Rect rect, SDL_Color color);
        ~Ball();


        int move(Pad* padL, Pad* padR);
        void setSpeed(int speed);

    private:
        std::mt19937 gen;
        std::uniform_int_distribution<int> dist;
        int speedX, speedY;

        void randomPos();
        int collisionWall();
        void colisionPad(Pad* pad);
};