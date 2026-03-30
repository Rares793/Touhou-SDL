#include "../include/collisionBox.h"


collisionBox::collisionBox(){
    screenHeight = 0;
    screenWidth = 0;
}

collisionBox::collisionBox(int screenWidth, int screenHeight){
    this->screenHeight = screenHeight;
    this->screenWidth = screenWidth;
}

void collisionBox::init(int screenHeight, int screenWidth){
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