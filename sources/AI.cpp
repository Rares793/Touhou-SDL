#include "../include/AI.h"

void ai::addMovement(MovementState mv){
    numberOfMovementComponents++;
    if(numberOfMovementComponents == 1){
        this->mv = new MovementState[1];
        this->mv[0] = mv;
        return;
    }

    MovementState* newMv = new MovementState[numberOfMovementComponents];
    for(int i = 0; i < numberOfMovementComponents - 1; i++)
        newMv[i] = this->mv[i];
    newMv[numberOfMovementComponents - 1] = mv;
    delete[] this->mv;
    this->mv = newMv;
}

void ai::init(){
    const int mvCount = 3;
    MovementState states[mvCount];

    // movement 0 — cubic bezier
    CubicBezier path(vec2(100, 100), vec2(500, 400), vec2(200, 50), vec2(400, 300));

    states[0].mv.set(path);
    
    // movement 1 — catmull rom
    states[1].mv.setCatmullRomSpline();
    states[1].mv.geometry.CRSpline.points.push(vec2(450,  400));  // phantom
    states[1].mv.geometry.CRSpline.points.push(vec2(500,  400));  // start
    states[1].mv.geometry.CRSpline.points.push(vec2(550, 200));
    states[1].mv.geometry.CRSpline.points.push(vec2(720, 300));
    states[1].mv.geometry.CRSpline.points.push(vec2(980, 150));
    states[1].mv.geometry.CRSpline.points.push(vec2(1080, 400));  // end
    states[1].mv.geometry.CRSpline.points.push(vec2(1080, 400));  // phantom

    // movement 2 — circle
    Circle circ(vec2(1080, 400), 100.0f);
    states[2].mv.set(circ);

    addMovement(states[0]);
    addMovement(states[1]);
    addMovement(states[2]);
}

void ai::handleMovement(enemy* en, float dt){
    if(currentState >= numberOfMovementComponents)
        return;

    mv[currentState].t         += en->getMovementSpeed() * dt;
    mv[currentState].totalTime += dt;
    if(mv[currentState].t >= 1.0f) {
        mv->isFinished = true;
        currentState++;
        if(currentState >= numberOfMovementComponents)
        return; // all movements done
    }

    vec2 nextPos; 

    switch(mv[currentState].mv.type){
        case MOVEMENT_TYPE::CUBIC_BEZIER:
            nextPos = mv[currentState].mv.geometry.CB.n_position(mv[currentState].t);
            en->setX(nextPos.x());
            en->setY(nextPos.y());
            break;
        case MOVEMENT_TYPE::CATMULL_ROLL:
            nextPos = mv[currentState].mv.geometry.CRSpline.n_position(mv[currentState].t);
            en->setX(nextPos.x());
            en->setY(nextPos.y());
            break;
        case MOVEMENT_TYPE::CIRCLE:
            nextPos = mv[currentState].mv.geometry.Circ.n_position(mv[currentState].t);
            en->setX(nextPos.x());
            en->setY(nextPos.y());
            break;
        case MOVEMENT_TYPE::DASH:
            nextPos = mv[currentState].mv.geometry.D.n_position(mv[currentState].t);
            en->setX(nextPos.x());
            en->setY(nextPos.y());
            break;
        case MOVEMENT_TYPE::IDLE:
            nextPos = mv[currentState].mv.geometry.Id.n_position(mv[currentState].t);
            en->setX(nextPos.x());
            en->setY(nextPos.y());
            break;
        case MOVEMENT_TYPE::SPIRAL:
            nextPos = mv[currentState].mv.geometry.Spir.n_position(mv[currentState].t);
            en->setX(nextPos.x());
            en->setY(nextPos.y());
            break;
        case MOVEMENT_TYPE::NONE:
            break;
        default: break;
    }
    
}