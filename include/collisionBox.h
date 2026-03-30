#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "player.h"
#include "enemy.h"

class  collisionBox{
    public:
    collisionBox(int screenWidth, int screenHeight);
    collisionBox();
    void checkCollisions(entity* e);

    void init(int screenHeight, int screenWidth);
    private:
    void checkCollisionWorld(entity* e);
    void updateVisibility(entity* e);
    int screenWidth = 0, screenHeight = 0;
};
#endif