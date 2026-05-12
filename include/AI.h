#ifndef AI 
#define AI

#include "enemy.h"
#include "movementGeometry.h"

class ai{
    public:
        void addMovement(MovementState mv);
        ai() : mv(nullptr), numberOfMovementComponents(0) {}
        ~ai() { delete[] mv; }
        
        void init();
        void handleMovement(enemy* en, float dt);

    private:
        MovementState* mv; // which kinda includes enemy state
        int numberOfMovementComponents = 0;
        int currentState = 0;
};
#endif 