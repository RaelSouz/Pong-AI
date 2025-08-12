#pragma once

#include <SDL2/SDL.h>

class Object {
    public:
        Object(SDL_FRect activeArea, SDL_FRect rect, SDL_Color color);
        virtual ~Object();

        void setRect(float w, float h, float x, float y);
        const SDL_FRect getRect() const;
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        const SDL_Color getColor() const;
        void setSpeed(float speed);
        const float getSpeed() const;
        void setPos(float x, float y);
        void draw(SDL_Renderer* renderer);

    protected:
        SDL_FRect rect, activeArea;
        SDL_Color color;
        SDL_Renderer* ptrRenderer;
        int win_w, win_h;
        float speed;
};