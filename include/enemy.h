#ifndef ENEMY_H
#define ENEMY_H

#include "movementGeometry.h"
#include "entity.h"


class enemy : public entity{
    public:
    void render(SDL_Renderer* renderer) override;
    void loadTexture(SDL_Renderer* renderer) override;
    void destroy() override;
    void init(SDL_Renderer* renderer) override;
    void setVisible();
    void setInvisible();
    void setTime(double time);
    double getTime();
    void update(double dt);
    enemy();
    enemy(float x, float y);

    private:

    protected:
  
    bool visible = false;
    double timeLeftScreen = 0.0;
    double totalTimeOffScreen = 0.0;
    static constexpr double maxInactiveTime = 2.0;
};
#endif