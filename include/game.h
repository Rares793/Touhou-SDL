#ifndef GAME_H
#define GAME_H

#include "collisionBox.h"
#include "player.h"
#include "enemy.h"

class Game{
public:
    bool init();
    void handleEvents();
    void SDL_AppQuit(void* appstate, SDL_AppResult result);
    SDL_AppResult SDL_AppIterate(void *appstate);
    void render();
    void clean();
    void run();
    void update();

protected:
    SDL_Renderer* renderer {nullptr};
    SDL_Window* window {nullptr};
    player Player;
    enemy Enemy;
    collisionBox cb;
    int screenWidth = 0, screenHeight = 0;
    bool running = false;
    double dt; // delta time
};
#endif