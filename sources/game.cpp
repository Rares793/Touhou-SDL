#include "../include/game.h"

bool Game::init(){
    SDL_SetAppMetadata("Touhou Wannabe", "0.1", "TH");
    
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Couldn't initialise SDL_Init, %s", SDL_GetError());
        return false;
    }

    if(!SDL_CreateWindowAndRenderer("Touhou", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer)){
        SDL_Log("Couldn't initialise window/renderer, %s", SDL_GetError());
        return false;
    }
    
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    running = true;
    Player.init(renderer);
    Enemy.en.init(renderer);
    Enemy.enAI.init();
    screenHeight = 1920;
    screenWidth = 1080;
    cb.init(screenHeight, screenWidth);
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
}

void Game::update(){
    Player.update(dt);
    Enemy.enAI.handleMovement(&Enemy.en, dt);
    Enemy.en.update(dt);
    cb.checkCollisions(&Player);
    cb.checkCollisions(&Enemy.en);
}

void Game::render(){
    const double now = static_cast<double>(SDL_GetTicks()) / 1000.0;
    const float red   = static_cast<float>(0.5 + 0.5 * SDL_sin(now));
    const float green = static_cast<float>(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2.0 / 3.0));
    const float blue  = static_cast<float>(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4.0 / 3.0));

    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    Player.render(renderer);
    Enemy.en.render(renderer);
    // Draw game objects here later

    SDL_RenderPresent(renderer);
}

void Game::run() {
    double lastTime = SDL_GetTicks();
    while (running) {
        double currentTime = SDL_GetTicks();
        dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (dt < 0.0001)
            dt = 0.0001;

        if (dt > 0.0333)
            dt = 0.0333;
        handleEvents();
        update();
        render();
        // crude frame cap ~60 FPS
        //SDL_Delay(16);
    }
}       

void Game::clean() {
    Player.destroy();
    Enemy.en.destroy();
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}