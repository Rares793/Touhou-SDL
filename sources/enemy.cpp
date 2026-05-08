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
    if(!texture)
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
    state = EnemyState::Entering;
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
void enemy::moveStraight(double dt){
    //v = vec2::random(); // already normalised
    vx = v.x() * movementSpeed;
    vy = v.y() * movementSpeed;

    x += vx * dt;
    y += vy * dt;   
}

float enemy::easeOutQuad(float t){
    return 1.0f - (1.0f - t) * (1.0f - t);
}

void enemy::moveTo(double dt){
    moveTimer += dt;
    float t = moveTimer / moveDuration;
    if (t > 1.0f)
        t = 1.0f;

    t = easeOutQuad(t);
    x = startPosition.x() + (targetPosition.x() - startPosition.x()) * t;
    y = startPosition.y() + (targetPosition.y() - startPosition.y()) * t;
    if (moveTimer >= moveDuration) {
        x = targetPosition.x();
        y = targetPosition.y();
        arrived = true;
    }
}

void enemy::moveSine(double dt, float baseX){
    y += 120.0f * dt;
    x = baseX + 80.0f * SDL_sin(stateTimer * dt);
}

void enemy::handleMovement(double dt, player* p){
    switch(state){
        case EnemyState::Entering:
            targetPosition = vec2(p->getX(), p->getY());
            moveTo(dt);
            if(arrived){
                state = EnemyState::Attacking;
                moveTimer = 0.0f;
                stateTimer = 0.0f;
                arrived = false;
            }
            break;
        case EnemyState::Attacking:
            stateTimer += dt;
            moveSine(dt, x);
            if(stateTimer > 2.0f)
                state = EnemyState::Leaving;
            break;
        case EnemyState::Leaving:
            v = vec2(0, -1);
            moveStraight(dt);
            break;
    }
}


void enemy::update(double dt, player* p){
    if(!alive)
        return;

    handleMovement(dt, p);
    
    // if an enemy has been off the screen for too long, kill it
    double now = static_cast<double>(SDL_GetTicks()) / 1000.0;
    if(now - timeLeftScreen > maxInactiveTime && timeLeftScreen != 0.0 && !visible && alive)
        destroy();
    
}