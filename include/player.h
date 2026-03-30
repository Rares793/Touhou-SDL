#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class player : public entity{
    public:
    void render(SDL_Renderer* renderer) override;
    void loadTexture(SDL_Renderer* renderer) override;
    void update() override;
    void destroy() override;
    void handleInput();
    void handleMovement();
    void init(SDL_Renderer* renderer) override;
    player();
    player(float x, float y);
};
#endif