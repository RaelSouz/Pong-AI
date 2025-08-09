#pragma once

#include <SDL2/SDL.h>

class Object {
    public:
        Object(SDL_Rect activeArea, SDL_Rect rect, SDL_Color color);
        virtual ~Object();

        void setRect(int w, int h, int x, int y);
        SDL_Rect getRect();
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        SDL_Color getColor();
        void setSpeed(int speed);
        int getSpeed();
        void setPos(int x, int y);
        void draw(SDL_Renderer* renderer);

    protected:
        SDL_Rect rect, activeArea;
        SDL_Color color;
        SDL_Renderer* ptrRenderer;
        int win_w, win_h;
        int speed;
};