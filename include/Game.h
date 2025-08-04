#pragma once

#include <SDL2/SDL.h>
#include <iostream>

#include <Objects/Pad.h>
#include <Objects/Ball.h>
#include <Ui/Label.h>

class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* background;
        SDL_Event event;

        int win_w, win_h;
        int scoreL, scoreR;
        int frameDelay;
        bool inGame, paused;
        Pad* padL;
        Pad* padR;
        Ball* ball;
        Font* scoreFont;
        Label* scoreLabelL;
        Label* scoreLabelR;

        void eventHandle();
        void drawScene();
        void renderScene();
        void createBackground();

    public:

        Game(int width, int height, int frameRate);
        ~Game();

        int init();
        void handle();
};