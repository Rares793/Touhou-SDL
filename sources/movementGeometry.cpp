#include "../include/movementGeometry.h"

// Cubic Bezier

CubicBezier::CubicBezier(vec2 A, vec2 B, vec2 C1, vec2 C2){
    this->A = A;
    this->B = B;
    this->C1 = C1;
    this->C2 = C2;
}


CubicBezier::CubicBezier(vec2 A, vec2 B){
    vec2 C1(100, 100), C2(100, 100);
    CubicBezier(A, B, A+C1, B+C2);
}

CubicBezier::CubicBezier(vec2 A, vec2 B, vec2 C){
    CubicBezier(A, B,C, C);
}

vec2 CubicBezier::n_position(float progress) const{
    if(progress < 0 || progress > 1)
        return errorVector;

    float u = 1-progress;
    return (u*u*u)*A + 3*(u*u)*progress*C1 + 3*(progress*progress)*u*C2 + (progress*progress*progress)*B;
}

vec2 CubicBezier::N_tangent(float progress) const{
    if(progress < 0 || progress > 1)
        return errorVector;

    float u = 1-progress;
    return (3*u*u*(C1-A) + 6*u*progress*(C2-C1) + 3*progress*progress*(B-C2)).normalised();
}

// Orbit

Circle::Circle(vec2 center, float radius){
    this->center = center;
    this->radius = radius;
}

vec2 Circle::n_position(float progress) const{
    if (progress < 0 || progress > 1)
        return errorVector;

    float angle = progress * 2 * M_PI;

    return vec2(center.x() + radius * SDL_cos(angle), center.y() + radius * SDL_sin(angle));
}

vec2 Circle::N_tangent(float progress) const{
    if(progress < 0 || progress > 1)
        return errorVector;

    float angle = progress * 2 * M_PI;

    return vec2(-SDL_sin(angle), SDL_cos(angle)); // normalised 
}

// Dash

Dash::Dash(vec2 start, vec2 end){
    this->start = start;
    this->end = end;
}

vec2 Dash::n_position(float progress) const{
    if(progress < 0 || progress > 1)
        return errorVector;

    return start + (end - start) * progress;
}

vec2 Dash::N_tangent(float progress) const{
    if(progress < 0 || progress > 1)
        return errorVector;

    return (end - start).normalised();
}

// Idle

Idle::Idle(vec2 anchor, float xAmplitude, float yAmplitude, float xFrequency, float yFrequency){
    this->anchor = anchor;
    this->xAmplitude = xAmplitude;
    this->yAmplitude = yAmplitude;
    this->xFrequency = xFrequency;
    this->yFrequency = yFrequency;
}

vec2 Idle::n_position(float time) const{
    return vec2(anchor.x() + xAmplitude * SDL_sin(xFrequency * time), anchor.y() + yAmplitude * SDL_sin(yFrequency * time));
}

vec2 Idle::N_tangent(float time) const{
    return vec2(xAmplitude * xFrequency * SDL_cos(xFrequency * time), yAmplitude * yFrequency * SDL_cos(yFrequency * time)).normalised();
}

// Catmull Rom Spline

vec2 CatmullRomSpline::n_computePosition(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, float t) const{
    return 0.5f * (2.0f * p1 + (p2 - p0)*t + (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3) * t * t + (3.0f*p1 - p0 - 3.0f*p2 + p3)*t*t*t);
}

vec2 CatmullRomSpline::n_position(float t) const{
    int segmentCount = points.size - 3;
    float scaled = t * segmentCount;
    int i = (int) scaled;
    float local = scaled - i;

    vec2 p0 = points.getNodeAtPosition(local).v;
    vec2 p1 = points.getNodeAtPosition(local + 1).v;
    vec2 p2 = points.getNodeAtPosition(local + 2).v;
    vec2 p3 = points.getNodeAtPosition(local + 3).v;

    return n_computePosition(p0, p1, p2, p3, t);
}

vec2 CatmullRomSpline::N_tangent(float t) const{
    float h = 0.001f;
    return (n_position(std::min(t+h, 1.0f)) - n_position(std::max(t-h,0.0f)).normalised()); 
}

// Spiral

vec2 Spiral::n_position(float t) const{
    float angle = t * orbitSpeed;
    float radius = startRadius + t * expansionSpeed;
    return vec2(center.x() + radius * SDL_cos(angle), center.y() + radius * SDL_sin(angle));
}

vec2 Spiral::N_tangent(float t) const{
    float h = 0.001f;
    return (n_position(t + h) - n_position(t -h)).normalised();
}

// Homing

void Homing::update(vec2& enemyPos, const vec2& playerPos, float dt){
    float targetAngle = SDL_atan2(playerPos.y() - enemyPos.y(),playerPos.x() - enemyPos.x());
    currentAngle = lerpAngle(currentAngle, targetAngle, turnSpeed * dt);
    enemyPos += vec2::polarToCartesian(currentAngle) * moveSpeed * dt;
}

vec2 Homing::N_tangent() const{
    return vec2::polarToCartesian(currentAngle);
}
// Extra functions

float lerpAngle(float a, float b, float t){
    float diff = b - a;
    while(diff > M_PI) 
        diff -= 2 * M_PI;
    while (diff < -M_PI) 
        diff += 2 * M_PI;
    return a + diff * t;
}