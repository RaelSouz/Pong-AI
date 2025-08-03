#include <Game.h>

#define BACKGROUND_COLOR 25, 25, 25, 25
#define PAD_WIDTH 15
#define PAD_HEIGHT 100
#define PAD_SPEED 6
#define PAD_COLOR {255, 255, 255, 255}
#define V_BORDER_H 15
#define V_DOT_H 30
#define V_DOT_W 15
#define V_BORDER_COLOR 170, 170, 170, 255
#define H_PADDING 60

Game::Game(int width, int height, int frameRate) {
    win_w = width, win_h = height;
    frameDelay = 1000 / frameRate;
    inGame = false;
    window = nullptr, renderer = nullptr, background = nullptr;
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

    if((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) { // Inicializando o renderer
        std::cerr << "Erro ao inicializar o renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    if((background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win_w, win_h)) == NULL) { // Inicializando o background
        std::cerr << "Erro ao inicializar a texxtura do background: " << SDL_GetError() << std::endl;
        return 1;
    }

    createBackground();

    // Definições Iniciais
    const int INITIAL_HEIGHT = (win_h / 2) - (PAD_HEIGHT / 2);
    const SDL_Rect activeArea = {.x = 0, .y = V_BORDER_H, .w = win_w, .h = win_h - (2 * V_BORDER_H)};
    padL = new Pad(
        renderer, activeArea,
        {.x = H_PADDING, .y = INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, PAD_COLOR
    );
    padL->setSpeed(PAD_SPEED);
    padR = new Pad(
        renderer, activeArea, 
        {.x = win_w - H_PADDING - PAD_WIDTH, .y = INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, PAD_COLOR
    );
    padR->setSpeed(PAD_SPEED);

    inGame = true;
    return 0;
}

void Game::eventHandle() {
    while(inGame && SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
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
    SDL_RenderCopy(renderer, background, NULL, NULL);
    padL->draw();
    padR->draw();
}

void Game::renderScene() {
    SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    SDL_Delay(frameDelay);
}

void Game::createBackground() {
    SDL_Texture* renderTargetOriginal = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, background);

    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
    SDL_RenderClear(renderer);

    SDL_Rect border = {.x = 0, .y = 0, .w = win_w, .h = V_BORDER_H};
    SDL_SetRenderDrawColor(renderer, V_BORDER_COLOR);
    SDL_RenderFillRect(renderer, &border);
    border.y = win_h - border.h;
    SDL_RenderFillRect(renderer, &border);

    int numDot = (win_h / V_DOT_H) / 2;
    int distance = (win_h - V_DOT_H) / numDot;
    SDL_Rect dot = {.x = (win_w / 2) - (V_DOT_H / 2), .y = V_DOT_H, .w = V_DOT_W, .h = V_DOT_H};
    for(int i = 0; i < numDot && dot.y < win_h; i++) {
        SDL_RenderFillRect(renderer, &dot);
        dot.y += distance;
    }

    SDL_SetRenderTarget(renderer, renderTargetOriginal);
}