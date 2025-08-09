#include <Game.h>

Game::Game(int width, int height, int frameRate) :
    win_w(width), win_h(height), scoreL(0), scoreR(0),
    frameDelay(1000 / frameRate), gameState(GameState::onInitialScene),
    window(nullptr), renderer(nullptr), initialBackground(nullptr), runningBackground(nullptr),
    event(), padL(nullptr), padR(nullptr), ball(nullptr),
    scoreFont(nullptr), titleFont(nullptr), pressKeyFont(nullptr), infoFont(nullptr),
    scoreLabelL(nullptr), scoreLabelR(nullptr), title(nullptr), pressKey(nullptr), infoPause(nullptr), infoQuit(nullptr),
    net()
{}

Game::~Game() {
    if(padL) delete padL;
    if(padR) delete padR;
    if(ball) delete ball;
    if(scoreLabelL) delete scoreLabelL;
    if(scoreLabelR) delete scoreLabelR;
    if(title) delete title;
    if(pressKey) delete pressKey;
    if(infoPause) delete infoPause;
    if(infoQuit) delete infoQuit;
    if(victoryTitle) delete victoryTitle;
    if(infoRestartSame) delete infoRestartSame;
    if(infoRestartNew) delete infoRestartNew;
    if(window) SDL_DestroyWindow(window);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(initialBackground) SDL_DestroyTexture(initialBackground);
    if(runningBackground) SDL_DestroyTexture(runningBackground);
    if(scoreFont) utils::closeFont(scoreFont);
    if(titleFont) utils::closeFont(titleFont);
    if(pressKeyFont) utils::closeFont(pressKeyFont);
    if(infoFont) utils::closeFont(infoFont);
    if(victoryFont) utils::closeFont(victoryFont);
    if(net) delete net;
    SDL_Quit();
}

int Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Erro ao inicializar o SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    if((window = SDL_CreateWindow(
        "Pong AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, 0
    )) == NULL) { 
        std::cerr << "Erro na criação da janela: " << SDL_GetError() << std::endl;
        return -1;
    }

    if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        std::cerr << "Erro na criação do renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

    if(utils::initFonts() < 0) {
        std::cerr << "Erro ao inicializar as fontes: " << SDL_GetError() << std::endl;
        return -1;
    }

    chdir("..");    // Retorna para o diretório do game
    if((scoreFont = utils::openFont(FONT_PATH, FONT_SCORE_SIZE)) == NULL) {
        std::cerr << "Erro ao abrir a fonte(PATH): " << FONT_PATH << " " << SDL_GetError() << std::endl;
        return -1;
    }

    if(createInitialScene() < 0) { 
        std::cerr << "Erro na criação da tela inicial: " << SDL_GetError() << std::endl;
        return -1;
    }

    if(createRunningScene() < 0) {
        std::cerr << "Erro na criação da tela de execução: " << SDL_GetError() << std::endl;
        return -1;
    }

    if(createEndScene() < 0) {
        std::cerr << "Erro na criação da tela vitória: " << SDL_GetError() << std::endl;
        return -1;
    }

    return 0;
}

int Game::handle() { 
    const Uint8* kbState = SDL_GetKeyboardState(NULL);  // Captura de teclas
    int sideCollision;
    vec input(3);
    double out;
    Uint32 currentTime, lastToggleTime = 0;
    bool showText = false;
  
    while(gameState != GameState::onQuit) {
        eventHandle();
        if(gameState == GameState::onInitialScene) {
            currentTime = SDL_GetTicks64();
            if(currentTime - lastToggleTime >= INITIAL_ANIM_TIME) {
                showText = !showText;
                lastToggleTime = currentTime;
            }
            SDL_RenderCopy(renderer, initialBackground, NULL, NULL);
            if(showText) pressKey->draw(renderer);
        }

        if(gameState == GameState::onGame) {
            if(scoreL == POINTS_WIN_CONDICION || scoreR == POINTS_WIN_CONDICION) {
                if(victoryTitle->setText(renderer, (
                    (scoreL == POINTS_WIN_CONDICION) ? FONT_VICTORY : FONT_LOSS
                )) < 0) return -1;
                victoryTitle->setPosition((win_w / 2) - (victoryTitle->getRect().w / 2), FONT_TITLE_PADDING);
                gameState = GameState::onEnd;
            }
            
            sideCollision = ball->move(padL, padR);
            if(sideCollision == 1) {
                scoreR++;
                updateScore(SCORE_RIGHT);
            } else if(sideCollision == 2) {
                scoreL++;
                updateScore(SCORE_LEFT);
                net->backPropagation(((input[1] < input[2] ) ? 1 : -1), AI_LEARN_RATE);    // Target = BallCenterY < PadCenterY ? 1 : -1;
            }

            SDL_PumpEvents();   // Atualização da captura de teclas
            // Jogador
            if(kbState[SDL_SCANCODE_W]) padL->move(true);
            else if(kbState[SDL_SCANCODE_S]) padL->move(false);
        
            // AI
            input[0] = nnet::normalizeTanh(ball->getRect().x + (ball->getRect().w/ 2), 0, win_w);   // BallCenterX
            input[1] = nnet::normalizeTanh(ball->getRect().y + (ball->getRect().h / 2), 0, win_h);  // BallCenterY
            input[2] = nnet::normalizeTanh(padR->getRect().y + (padR->getRect().h / 2), 0, win_h);  // PadCenterY
            out = net->feedFoward(input);
            if(out > 0.01) padR->move(true);    // Move Up
            else if(out < -0.01) padR->move(false); // Move Down
    
            SDL_RenderCopy(renderer, runningBackground, NULL, NULL);
            scoreLabelL->draw(renderer);
            scoreLabelR->draw(renderer);
            ball->draw(renderer);
            padL->draw(renderer);
            padR->draw(renderer);
        }

        if(gameState == GameState::onEnd) {
            SDL_RenderCopy(renderer, endBackground, NULL, NULL);
            victoryTitle->draw(renderer);
        }

        renderScene();
    }
    return 0;
}

void Game::eventHandle() {
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                gameState = GameState::onQuit;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.scancode == SDL_SCANCODE_Q) {   // (Q) para sair
                    gameState = GameState::onQuit;
                } else if(gameState == GameState::onInitialScene && event.key.keysym.scancode == SDL_SCANCODE_SPACE) { // (Espaço) para iniciar
                    gameState = GameState::onGame;  
                } else if(event.key.keysym.scancode == SDL_SCANCODE_P) {  // (P) para pausar
                    if(gameState == GameState::onGame) {
                        gameState = GameState::onPaused;
                    } else if (gameState == GameState::onPaused) {
                        gameState = GameState::onGame;
                    }
                } else if(gameState == GameState::onEnd) {
                    if(event.key.keysym.scancode == SDL_SCANCODE_M) {
                        onRestart(true);
                    } else if (event.key.keysym.scancode == SDL_SCANCODE_N) {
                        onRestart(false);
                    }
                }
                break;
        };
    }
}

int Game::createInitialScene() {
    if((initialBackground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win_w, win_h)) == NULL) return -1;
    if((titleFont = utils::openFont(FONT_PATH, FONT_TITLE_SIZE)) == NULL) return -1;
    if((pressKeyFont = utils::openFont(FONT_PATH, FONT_PRESS_KEY_SIZE)) == NULL) return -1;
    if((infoFont = utils::openFont(FONT_PATH, FONT_INFO_SIZE)) == NULL) return -1;

    SDL_SetRenderTarget(renderer, initialBackground);
    title = new Label(titleFont, {FONT_TITLE_COLOR});
    infoPause = new Label(infoFont, {FONT_INFO_COLOR});
    infoQuit = new Label(infoFont, {FONT_INFO_COLOR});
    
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
    SDL_RenderClear(renderer);

    if(title->setText(renderer, FONT_TITLE) < 0) return -1;
    if(infoPause->setText(renderer, FONT_INFO_P) < 0) return -1;
    if(infoQuit->setText(renderer, FONT_INFO_Q) < 0) return -1;

    title->setPosition((win_w / 2) - (title->getRect().w / 2), FONT_TITLE_PADDING);
    infoQuit->setPosition(FONT_INFO_PADDING, win_h - infoQuit->getRect().h - FONT_INFO_PADDING);
    infoPause->setPosition(FONT_INFO_PADDING, infoQuit->getRect().y - FONT_INFO_PADDING - infoPause->getRect().h);

    title->draw(renderer);
    infoPause->draw(renderer);
    infoQuit->draw(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    pressKey = new Label(pressKeyFont, {FONT_PRESS_KEY_COLOR});
    if(pressKey->setText(renderer, FONT_PRESS_KEY) < 0) return -1;
    pressKey->setPosition((win_w / 2) - (pressKey->getRect().w / 2), FONT_PRESS_KEY_PADDING);
    return 0;
}

int Game::createRunningScene() {
    if((runningBackground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win_w, win_h)) == NULL) return -1;
    SDL_SetRenderTarget(renderer, runningBackground);

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

    SDL_SetRenderTarget(renderer, NULL);

    const SDL_Rect ACTIVE_AREA{.x = 0, .y = V_BORDER_H, .w = win_w, .h = win_h - (2 * V_BORDER_H)};
    padL = new Pad(
        ACTIVE_AREA,
        {.x = H_PADDING, .y = PAD_INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, {PAD_COLOR}
    );
    padL->setSpeed(PAD_SPEED);
    padR = new Pad(
        ACTIVE_AREA, 
        {.x = win_w - H_PADDING - PAD_WIDTH, .y = PAD_INITIAL_HEIGHT, .w = PAD_WIDTH, .h = PAD_HEIGHT}, {PAD_COLOR}
    );
    padR->setSpeed(PAD_SPEED);

    ball = new Ball(
        ACTIVE_AREA,
        {.w = BALL_SIZE, .h = BALL_SIZE}, {BALL_COLOR}
    );
    ball->setSpeed(BALL_SPEED);

    scoreLabelL = new Label(scoreFont, {FONT_SCORE_COLOR});
    scoreLabelR = new Label(scoreFont, {FONT_SCORE_COLOR});
    updateScore(SCORE_LEFT);
    updateScore(SCORE_RIGHT);
    net = new NNet();
    return 0;
}

int Game::createEndScene() {
    if((endBackground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win_w, win_h)) == NULL) return -1;
    if((victoryFont = utils::openFont(FONT_PATH, FONT_VICTORY_SIZE)) == NULL) return -1;

    SDL_SetRenderTarget(renderer, endBackground);
    victoryTitle = new Label(victoryFont, {FONT_TITLE_COLOR});
    infoRestartSame = new Label(infoFont, {FONT_INFO_COLOR});
    infoRestartNew = new Label(infoFont, {FONT_INFO_COLOR});
    
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
    SDL_RenderClear(renderer);

    if(infoRestartSame->setText(renderer, FONT_INFO_M) < 0) return -1;
    if(infoRestartNew->setText(renderer, FONT_INFO_N) < 0) return -1;

    infoRestartNew->setPosition(FONT_INFO_PADDING, infoPause->getRect().y - FONT_INFO_PADDING - infoRestartNew->getRect().h);
    infoRestartSame->setPosition(FONT_INFO_PADDING, infoRestartNew->getRect().y - FONT_INFO_PADDING - infoRestartSame->getRect().h);

    infoRestartSame->draw(renderer);
    infoRestartNew->draw(renderer);
    infoPause->draw(renderer);
    infoQuit->draw(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    return 0;
}

int Game::updateScore(bool isLeft) {
    if(isLeft) {
        if(scoreLabelL->setText(renderer, std::to_string(scoreL)) < 0) return -1;
        scoreLabelL->setPosition((win_w / 2) - H_SCORE_PADDING - scoreLabelL->getRect().w, V_SCORE_PADDING);
    }
    else {
        if(scoreLabelR->setText(renderer, std::to_string(scoreR)) < 0) return -1;
        scoreLabelR->setPosition((win_w / 2) + H_SCORE_PADDING, V_SCORE_PADDING);
    }
    return 0;
}

void Game::onRestart(bool isSame) {
    padL->setPos(padL->getRect().x, PAD_INITIAL_HEIGHT);
    padR->setPos(padR->getRect().x, PAD_INITIAL_HEIGHT);
    ball->randomPos();
    scoreL = 0, scoreR = 0;
    updateScore(SCORE_LEFT);
    updateScore(SCORE_RIGHT);
    if(!isSame) {
        delete net;
        net = new NNet();
    }
    gameState = GameState::onGame;
}

void Game::renderScene() {
    SDL_RenderPresent(renderer);
    SDL_Delay(frameDelay);
}