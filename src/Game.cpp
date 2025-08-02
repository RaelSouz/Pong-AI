#include <Game.h>

#define LATERAL_DISTANCE_WALL 60
#define PAD_WIDTH 15
#define PAD_HEIGHT 100
#define PAD_SPEED 6

Game::Game(int width, int height, int frameRate) {
    win_w = width, win_h = height;
    frameDelay = 1000 / frameRate;
    inGame = false;
}

Game::~Game() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    delete padL;
    delete padR;
    SDL_Quit();
}

int Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Inicializando o SDL
        std::cerr << "Erro ao inicializar o SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if((window = SDL_CreateWindow(  // Inicialização da Janela
        "Pong AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, 0
    )) == NULL) { 
        std::cerr << "Erro ao inicializar a janela: " << SDL_GetError() << std::endl;
        return 1;
    }

    if((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) { // Criação do renderer
        std::cerr << "Erro ao inicializar o renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    const int INITIAL_HEIGHT = (win_h / 2) - (PAD_HEIGHT / 2);
    const SDL_Color PAD_COLOR = {255, 255, 255, 255};
    padL = new Pad(
        window, renderer, 
        {.x = LATERAL_DISTANCE_WALL, .y = INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, PAD_COLOR
    );
    padL->setSpeed(PAD_SPEED);
    padR = new Pad(
        window, renderer, 
        {.x = win_w - LATERAL_DISTANCE_WALL - PAD_WIDTH, .y = INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, PAD_COLOR
    );
    padR->setSpeed(PAD_SPEED);

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
    const Uint8* kbState = SDL_GetKeyboardState(NULL);  // Captura de teclas
    while(inGame) {
        eventHandle();
        
        SDL_PumpEvents();   // Atualização da captura de teclas

        if(kbState[SDL_SCANCODE_W]) padL->move(true);   // Teclas jogador 1
        else if(kbState[SDL_SCANCODE_S]) padL->move(false);
        if(kbState[SDL_SCANCODE_UP]) padR->move(true);  // Teclas jogador 2
        else if(kbState[SDL_SCANCODE_DOWN]) padR->move(false);

        drawScene();
        renderScene();
    }
}

void Game::drawScene() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 25);  // Background #141414
    SDL_RenderClear(renderer);
    padL->draw();
    padR->draw();
}

void Game::renderScene() {
    SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    SDL_Delay(frameDelay);
}