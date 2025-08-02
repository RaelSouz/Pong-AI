#pragma once

#include <SDL2/SDL.h>
#include <iostream>

class Game {
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Event event;

        int window_w, window_h;
        int frameDelay;
        bool inGame;

        void eventHandle();
        void drawScene();
        void renderScene();

    public:

        Game(int width, int height, int frameRate);
        ~Game();

        int init();
        void handle();
};