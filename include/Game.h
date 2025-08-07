#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>

#include <Objects/Pad.h>
#include <Objects/Ball.h>
#include <Ui/Label.h>
#include <NeuralNetwork/NNet.h>

enum class GameState {
    onMainScene,
    onGame,
    onPaused,
    onQuit
};

class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* background;
        SDL_Texture* mainScene;
        SDL_Event event;
        SDL_Rect activeArea;

        int win_w, win_h;
        int scoreL, scoreR;
        int frameDelay;
        GameState gameState;
        Pad* padL;
        Pad* padR;
        Ball* ball;
        Font* scoreFont;
        Label* scoreLabelL;
        Label* scoreLabelR;
        NNet net;
        

        void quit();
        void eventHandle();
        void drawScene();
        void renderScene();
        void createBackground();
        void createMainScene();
        void updateScore(bool isLeft);

    public:

        Game(int width, int height, int frameRate);
        ~Game();

        int init();
        void handle();
};