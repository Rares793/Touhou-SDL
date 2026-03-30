#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "../include/entity.h"
class  collisionBox{
    public:
    void checkCollisionWorld(entity* e);
    collisionBox(int screenWidth, int screenHeight);
    collisionBox();
    void checkCollisions(entity* e);
    void init(int screenHeight, int screenWidth);
    private:
    int screenWidth = 0, screenHeight = 0;
};
#endif