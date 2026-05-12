#include "../include/player.h"

void player::loadTexture(SDL_Renderer* renderer) {
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

void player::render(SDL_Renderer* renderer){
    if(!texture)
        return;

    SDL_FRect rect = getRect();
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

void player::handleInput() {
    //should be vectorised later maybe
    float dx = 0.0f;
    float dy = 0.0f;

    const bool* currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) 
        dx -= 1.0f;
    if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) 
        dx += 1.0f;
    if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W]) 
        dy -= 1.0f;
    if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S]) 
        dy += 1.0f;

    float length = SDL_sqrt(dx * dx + dy * dy);
    if (length > 0.0f) {
        dx /= length;
        dy /= length;
    }

    vx = dx * movementSpeed;
    vy = dy * movementSpeed;
}

player::player(){
    alive = true;
}

player::player(float x, float y){
    this->x = x;
    this->y = y;
    alive = true;
}

void player::handleMovement(double dt){
    handleInput();

    x += vx * dt;
    y += vy * dt;
}

void player::update(double dt){
    handleMovement(dt);
}

void player::init(SDL_Renderer* renderer){
    loadTexture(renderer);
    movementSpeed = 300.0f;
}

void player::destroy(){
    if(texture){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    alive = false;
}