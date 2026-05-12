#include "../include/enemy.h"

void enemy::loadTexture(SDL_Renderer* renderer) {
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

void enemy::render(SDL_Renderer* renderer){
    // if no texture, don't try to render
    if(!texture || !visible)
        return;

    SDL_FRect rect = getRect();
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

enemy::enemy(){
    alive = true;
}

enemy::enemy(float x, float y){
    this->x = x;
    this->y = y;
    alive = true;
}

void enemy::init(SDL_Renderer* renderer){
    loadTexture(renderer);
    alive = true;
}

void enemy::destroy(){
    if(texture){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    alive = false;
    printf("Enemy has been destroyed\n");
}

void enemy::setInvisible(){
    this->visible = false;
}

void enemy::setVisible(){
    this->visible = true;
}

void enemy::setTime(double time){
    this->timeLeftScreen = time;
}

double enemy::getTime(){
    return this->timeLeftScreen;
}

// the mmovement types will be moved in movementGeometry.h, while the AI logic will be its' own seperate class


void enemy::update(double dt){
    if(!alive)
        return;
    
    // if an enemy has been off the screen for too long, kill it
    double now = static_cast<double>(SDL_GetTicks()) / 1000.0;
    if(now - timeLeftScreen > maxInactiveTime && timeLeftScreen != 0.0 && !visible && alive)
        destroy();
    
}