#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"
#include "vec2.h"

enum class MoveType{
    Straight,
    MoveTo,
    SineDown,
    Idle
};

enum class EnemyState{
    Entering,
    Attacking,
    Leaving
};

class enemy : public entity{
    public:
    void render(SDL_Renderer* renderer) override;
    void loadTexture(SDL_Renderer* renderer) override;
    void destroy() override;
    void handleMovement(double dt, player* p);
    void init(SDL_Renderer* renderer) override;
    void setInvisible();
    void setVisible();
    void setTime(double time);
    double getTime();
    void update(double dt, player* p);
    enemy();
    enemy(float x, float y);

    private:
    void moveStraight(double dt);
    void moveTo(double dt);
    void moveSine(double dt, float baseX);
    float easeOutQuad(float t);

    protected:
    vec2 v;
    vec2 startPosition;
    vec2 targetPosition;
    float moveDuration = 5.0f;
    float moveTimer = 0.0f, stateTimer = 0.0f;
    MoveType moves;
    EnemyState state;   
    bool visible = false;
    bool arrived = false;
    double timeLeftScreen = 0.0;
    double totalTimeOffScreen = 0.0;
    static constexpr double maxInactiveTime = 2.0;
};
#endif