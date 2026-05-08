#include "../include/bullet.h"

void bullet::init(SDL_Renderer* renderer){
    loadTexture(renderer);
}

void bullet::destroy(){
    if(texture){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    alive = false;
}

void bullet::loadTexture(SDL_Renderer* renderer) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    texture = IMG_LoadTexture(renderer, "assets/player.png");
    if (!texture) {
        SDL_Log("IMG_LoadTexture failed: %s", SDL_GetError());
        return;
    }

    SDL_GetTextureSize(texture, &renderWidth, &renderHeight);
    x = SCREEN_WIDTH / 2.0f;
    y = SCREEN_HEIGHT - 100.0f;
}

void bullet::moveStraight(double dt, vec2 dir){
    vx = dir.x() * movementSpeed;
    vy = dir.y() * movementSpeed;

    x += vx * dt;
    y += vy * dt;   
}

void bullet::update(double dt){
    if(!alive)
        return;

    // if a bullet has been off the screen for too long, kill it
    double now = static_cast<double>(SDL_GetTicks()) / 1000.0;
    if(now - timeLeftScreen > maxInactiveTime && timeLeftScreen != 0.0 && !visible && alive)
        destroy();
}