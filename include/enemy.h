#ifndef ENEMY_H
#define ENEMY_H
#include "entity.h"
class enemy : entity{
    public:
    void render(SDL_Renderer* renderer) override;
    void loadTexture(SDL_Renderer* renderer) override;
    void destroy() override;
    void handleMovement();
    void init(SDL_Renderer* renderer) override;
    enemy();
    enemy(float x, float y);
};
#endif