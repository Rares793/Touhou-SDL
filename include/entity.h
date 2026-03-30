#ifndef ENTITY_H
#define ENTITY_H

#include "utilities.h"

class entity{
    public:
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void loadTexture(SDL_Renderer* renderer) = 0;
    virtual void init(SDL_Renderer* renderer) = 0;
    virtual void update() = 0;
    //virtual void move() = 0;
    virtual void destroy() = 0;
    float getX() const{ return x; }

    float getY() const{ return y; }

    float getVX() const{ return vx; }

    float getVY() const{ return vy; }

    float getWidth() const{ return renderWidth; }

    float getHeight() const{ return renderHeight; }

    void setX(float x){ this->x = x; }

    void setY(float y){ this->y = y; }

    void setVX(float vx){ this->vx = vx; }

    void setVY(float vy){ this->vy = vy; }

    void setWidth(float renderWidth){ this->renderWidth = renderWidth; }

    void setHeight(float renderHeight){ this->renderHeight = renderHeight; }

    SDL_Texture* getTexture(){ return this->texture; }

    void setTexture(SDL_Texture* texture){ this->texture = texture; }

    void setAlive(){ alive = true; }

    bool getAlive() const{ return alive; }

    SDL_FRect getRect() const {
    return SDL_FRect{
        x - renderWidth / 2.0f,
        y - renderHeight / 2.0f,
        renderWidth,
        renderHeight
    };
}
    virtual ~entity() = default;

    protected:
    float x = 0.0, y = 0.0;
    float vx = 0.0, vy = 0.0;
    float renderWidth = 0.0, renderHeight = 0.0;
    bool alive = false;
    float movementSpeed = 20.0;
    SDL_Texture* texture {nullptr};
};
#endif