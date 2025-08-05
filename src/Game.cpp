#include <Game.h>

#define BACKGROUND_COLOR 25, 25, 25, 25
#define PAD_WIDTH 15
#define PAD_HEIGHT 100
#define PAD_SPEED 7
#define PAD_COLOR 255, 255, 255, 255
#define BALL_SIZE 12
#define BALL_SPEED 6
#define BALL_COLOR 200, 200, 200, 255
#define V_BORDER_H 15
#define V_DOT_H 30
#define V_DOT_W 15
#define V_BORDER_COLOR 170, 170, 170, 255
#define H_PADDING 20
#define FONT_PATH "assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf"
#define FONT_SCORE_SIZE 40
#define FONT_SCORE_COLOR 170, 170, 170, 255
#define FONT_MAIN_TITLE "Pong AI"
#define FONT_MAIN_TITLE_SIZE 82
#define FONT_MAIN_TITLE_COLOR 240, 240, 240, 255
#define FONT_MAIN_PRESS_KEY "Pressione qualquer tecla para iniciar"
#define FONT_MAIN_PRESS_KEY_SIZE 14
#define FONT_MAIN_PRESS_KEY_COLOR 200, 200, 200, 255
#define V_SCORE_PADDING 60
#define H_SCORE_PADDING 60

#define SCORE_LEFT true
#define SCORE_RIGHT false

Game::Game(int width, int height, int frameRate) {
    win_w = width, win_h = height;
    scoreL = 0, scoreR = 0;
    frameDelay = 1000 / frameRate;
    gameState = GameState::onMainScene;
    window = nullptr, renderer = nullptr, background = nullptr;
    padL = nullptr, padR = nullptr, ball = nullptr;
    scoreFont = nullptr, scoreLabelL = nullptr, scoreLabelR = nullptr;
}

Game::~Game() {
    delete padL;
    delete padR;
    delete ball;
    delete scoreLabelL;
    delete scoreLabelR;
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
        std::cerr << "Erro ao inicializar a textura do background: " << SDL_GetError() << std::endl;
        return 1;
    }

    if(utils::initFonts() == 1) { // Criação do renderer
        std::cerr << "Erro ao inicializar as fonts: " << SDL_GetError() << std::endl;
        return 1;
    }

    chdir("..");    // Retorna para o diretório do game
    if((scoreFont = utils::openFont(FONT_PATH, FONT_SCORE_SIZE)) == NULL) { // Criação do renderer
        std::cerr << "Erro ao abrir a fonte: " << FONT_PATH << " " << SDL_GetError() << std::endl;
        return 1;
    }
    createMainScene();
    createBackground();

    // Definições Iniciais
    const int INITIAL_HEIGHT = (win_h / 2) - (PAD_HEIGHT / 2);
    const SDL_Rect activeArea = {.x = 0, .y = V_BORDER_H, .w = win_w, .h = win_h - (2 * V_BORDER_H)};
    padL = new Pad(
        renderer, activeArea,
        {.x = H_PADDING, .y = INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, {PAD_COLOR}
    );
    padL->setSpeed(PAD_SPEED);
    padR = new Pad(
        renderer, activeArea, 
        {.x = win_w - H_PADDING - PAD_WIDTH, .y = INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, {PAD_COLOR}
    );
    padR->setSpeed(PAD_SPEED);

    ball = new Ball(
        renderer, activeArea,
        {.w = BALL_SIZE, .h = BALL_SIZE}, {BALL_COLOR}
    );
    ball->setSpeed(BALL_SPEED);

    scoreLabelL = new Label(renderer, scoreFont, {FONT_SCORE_COLOR});
    updateScore(SCORE_LEFT);
    scoreLabelR = new Label(renderer, scoreFont, {FONT_SCORE_COLOR});
    updateScore(SCORE_RIGHT);

    return 0;
}

void Game::quit() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(mainScene);
    utils::closeFont(scoreFont);
    SDL_Quit();
}

void Game::eventHandle() {
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                gameState = GameState::onQuit;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.scancode == SDL_SCANCODE_Q) {   // Pressione Q para sair
                    gameState = GameState::onQuit;
                } else if(gameState == GameState::onMainScene) { // Pressione qualquer tecla para iniciar
                    gameState = GameState::onGame;  
                } else if(event.key.keysym.scancode == SDL_SCANCODE_P) {  // Pressione P para pausar
                    if(gameState == GameState::onGame) {
                        gameState = GameState::onPaused;
                    } else if (gameState == GameState::onPaused) {
                        gameState = GameState::onGame;
                    }
                }
                break;
        };
    }
}

void Game::handle() { 
    const Uint8* kbState = SDL_GetKeyboardState(NULL);  // Captura de teclas
    int sideCollision;

    // Tela inicial
    SDL_RenderCopy(renderer, mainScene, NULL, NULL);
    renderScene();
    while(gameState == GameState::onMainScene) {
        eventHandle();   
    }
  
    while(gameState != GameState::onQuit) {
        eventHandle();
        
        if(gameState == GameState::onGame) {
            SDL_PumpEvents();   // Atualização da captura de teclas
            if(kbState[SDL_SCANCODE_W]) padL->move(true);   // Teclas jogador 1
            else if(kbState[SDL_SCANCODE_S]) padL->move(false);
            if(kbState[SDL_SCANCODE_UP]) padR->move(true);  // Teclas jogador 2
            else if(kbState[SDL_SCANCODE_DOWN]) padR->move(false);  
            
            sideCollision = ball->move(padL, padR);
            if(sideCollision == 1) {
                updateScore(SCORE_LEFT);
            } else if(sideCollision == 2) {
                updateScore(SCORE_RIGHT);
            }  

            drawScene();
            renderScene();
        }
    }
    quit();
}

void Game::drawScene() {
    SDL_RenderCopy(renderer, background, NULL, NULL);
    scoreLabelL->draw();
    scoreLabelR->draw();
    padL->draw();
    padR->draw();
    ball->draw();
}

void Game::renderScene() {
    SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    SDL_Delay(frameDelay);
}

void Game::createMainScene() {
    SDL_Texture* renderTargetOriginal = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, mainScene);

    Font* titleFont = utils::openFont(FONT_PATH, FONT_MAIN_TITLE_SIZE);
    Font* pressKeyFont = utils::openFont(FONT_PATH, FONT_MAIN_PRESS_KEY_SIZE);
    Label* title = new Label(renderer, titleFont, {FONT_MAIN_TITLE_COLOR});
    Label* pressKey = new Label(renderer, pressKeyFont, {FONT_MAIN_PRESS_KEY_COLOR});
    
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
    SDL_RenderClear(renderer);
    title->setText(FONT_MAIN_TITLE);
    pressKey->setText(FONT_MAIN_PRESS_KEY);
    title->setPosition((win_w / 2) - (title->getRect().w / 2), 180);
    pressKey->setPosition((win_w / 2) - (pressKey->getRect().w / 2), 360);
    title->draw();
    pressKey->draw();

    SDL_SetRenderTarget(renderer, renderTargetOriginal);
    utils::closeFont(titleFont);
    utils::closeFont(pressKeyFont);
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
    SDL_Rect dot = {.x = (win_w / 2) - (V_DOT_W / 2), .y = V_DOT_H, .w = V_DOT_W, .h = V_DOT_H};
    for(int i = 0; i < numDot && dot.y < win_h; i++) {
        SDL_RenderFillRect(renderer, &dot);
        dot.y += distance;
    }

    SDL_SetRenderTarget(renderer, renderTargetOriginal);
}

void Game::updateScore(bool isLeft) {
    if(isLeft) {
        scoreL++;
        scoreLabelL->setText(std::to_string(scoreL));
        scoreLabelL->setPosition((win_w / 2) + H_SCORE_PADDING, V_SCORE_PADDING);
    }
    else {
        scoreR++;
        scoreLabelR->setText(std::to_string(scoreR));
        scoreLabelR->setPosition((win_w / 2) - H_SCORE_PADDING - scoreLabelL->getRect().w, V_SCORE_PADDING);
    }
}