#ifndef PLAYER_H
#define PLAYER_H

#include "collisionBox.h"
#include "entity.h"

class player : entity{
    public:
    void render(SDL_Renderer* renderer) override;
    void loadTexture(SDL_Renderer* renderer) override;
    void destroy() override;
    void handleInput();
    void handleMovement(collisionBox cb);
    void init(SDL_Renderer* renderer) override;
    player();
    player(float x, float y);
};
#endif