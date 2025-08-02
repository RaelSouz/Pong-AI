#include <Game.h>

Game::Game(int width, int height, int frameRate) {
    window_w = width, window_h = height;
    frameDelay = 1000 / frameRate;
    inGame = false;
}

Game::~Game() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

int Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Inicializando o SDL
        std::cerr << "Erro ao inicializar o SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if((window = SDL_CreateWindow(  // Inicialização da Janela
        "Pong AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, 0
    )) == NULL) { 
        std::cerr << "Erro ao inicializar a janela: " << SDL_GetError() << std::endl;
        return 1;
    }

    if((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) { // Criação do renderer
        std::cerr << "Erro ao inicializar o renderer: " << SDL_GetError() << std::endl;
        return 1;
    }
    inGame = true;
    return 0;
}

void Game::eventHandle() {
    while(inGame && SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                std::cout << "Programa finalizado." << std::endl;
                inGame = false;
                break;
        };
    }
}

void Game::handle() { 
    while(inGame) {
        eventHandle();
        drawScene();
        renderScene();
    }
}

void Game::drawScene() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 25);  // Background #141414
    SDL_RenderClear(renderer);
}

void Game::renderScene() {
    SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    SDL_Delay(frameDelay);
}