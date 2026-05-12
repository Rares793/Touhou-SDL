#include "../include/collisionBox.h"


collisionBox::collisionBox(){
    screenHeight = 0;
    screenWidth = 0;
}

collisionBox::collisionBox(int screenWidth, int screenHeight){
    this->screenHeight = screenHeight;
    this->screenWidth = screenWidth;
}

void collisionBox::init(int screenWidth, int screenHeight){
    this->screenHeight = screenHeight;
    this->screenWidth = screenWidth;
}

void collisionBox::checkCollisionWorld(entity* e){
    float halfW = e->getWidth() / 2.0f;
    float halfH = e->getHeight() / 2.0f;

    float x = e->getX();
    float y = e->getY();
    if (x - halfW < 0) {
        e->setX(halfW);
        e->setVX(0);
    }

    if (x + halfW > screenWidth) {
        e->setX(screenWidth - halfW);
        e->setVX(0);
    }

    if (y - halfH < 0) {
        e->setY(halfH);
        e->setVY(0);
    }

    if (y + halfH > screenHeight) {
        e->setY(screenHeight - halfH);
        e->setVY(0);
    }
}

void collisionBox::updateVisibility(entity* e){
    enemy* en = dynamic_cast<enemy*>(e);
    if(!en)
        return;

    float halfW = en->getWidth() / 2.0f;
    float halfH = en->getHeight() / 2.0f;

    float x = en->getX();
    float y = en->getY();

    if (x + halfW < 0) {
        en->setInvisible();
        if(en->getTime() == 0)
            en->setTime(static_cast<double>(SDL_GetTicks()) / 1000.0);
        return;
    }

    if (x - halfW > screenWidth) {
        en->setInvisible();
        if(en->getTime() == 0)
            en->setTime(static_cast<double>(SDL_GetTicks()) / 1000.0);
        return;
    }

    if (y + halfH < 0) {
        en->setInvisible();
        if(en->getTime() == 0)
            en->setTime(static_cast<double>(SDL_GetTicks()) / 1000.0);
        return;
    }

    if (y - halfH > screenHeight) {
        en->setInvisible();
        if(en->getTime() == 0)
            en->setTime(static_cast<double>(SDL_GetTicks()) / 1000.0);
        return;
    }

    en->setVisible();
    en->setTime(0.0);
}

void collisionBox::checkCollisions(entity* e){
    player* p = dynamic_cast<player*>(e);
    // checks all collisions that the Player class should care about
    if(p){ 
        checkCollisionWorld(p);
        return;
    }

    enemy* en = dynamic_cast<enemy*>(e);
    // checks all collisions that the Enemy class should care about
    if(en){
        updateVisibility(en);
        return;
    }

    bullet* b = dynamic_cast<bullet*>(e);
    // checks all collisions that the Bullet class should care about
    if(b){
        updateVisibility(b);
        return;
    }
}