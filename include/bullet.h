#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

class bullet : public entity{
public:
    void render(SDL_Renderer* renderer) override;
    void loadTexture(SDL_Renderer* renderer) override;
    void init(SDL_Renderer* renderer) override;
    void update(double dt);
    void destroy() override;
    void handleMovement(double dt);

private:
    void moveStraight(double dt, vec2 dir);

protected:
    bool visible = false;
    double timeLeftScreen = 0.0;
    double totalTimeOffScreen = 0.0;
    static constexpr double maxInactiveTime = 2.0;
};
#endif
