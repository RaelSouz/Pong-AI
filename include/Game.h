#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>

#include <Config.h>
#include <Objects/Pad.h>
#include <Objects/Ball.h>
#include <Ui/Label.h>
#include <NeuralNetwork/NNet.h>

enum class GameState {
    onInitialScene,
    onGame,
    onPaused,
    onQuit
};

class Game {
    private:

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* initialBackground;
        SDL_Texture* runningBackground;
        SDL_Event event;

        int win_w, win_h;
        int scoreL, scoreR;
        int frameDelay;
        GameState gameState;
        Pad* padL;
        Pad* padR;
        Ball* ball;
        Font* scoreFont;
        Font* titleFont;
        Font* pressKeyFont;
        Font* infoFont;
        Label* scoreLabelL;
        Label* scoreLabelR;
        Label* title;
        Label* pressKey;
        Label* infoPause;
        Label* infoQuit;
        NNet net;
        

        void eventHandle();
        int createInitialScene();
        int createRunningScene();
        void updateScore(bool isLeft);
        void renderScene();

    public:

        Game(int width, int height, int frameRate);
        ~Game();

        int init();
        void handle();
};