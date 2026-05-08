#ifndef MOVEMENT_GEOMETRY
#define MOVEMENT_GEOMETRY

#include "vec2.h"
#include "pointList.h"

// progress is in the [0,1] interval

enum MOVEMENT_TYPE{CIRCLE, DASH, CUBIC_BEZIER, IDLE, SPIRAL, CATMULL_ROLL, HOMING};

struct CubicBezier{
    vec2 A; // A - start point of Bezier Curve 
    vec2 B; // end point of Bezier Curve
    vec2 C1, C2; // control points

    CubicBezier(vec2 A, vec2 B);
    CubicBezier(vec2 A, vec2 B, vec2 C);
    CubicBezier(vec2 A, vec2 B, vec2 C1, vec2 C2);


    vec2 n_position(float progress) const;
    vec2 N_tangent(float progress) const;
};

struct Circle{
    vec2 center;
    float radius;

    Circle(vec2 center, float radius);
    vec2 n_position(float progress) const;
    vec2 N_tangent(float progress) const;
};

struct Dash{
    vec2 start, end;

    Dash(vec2 start, vec2 end);

    vec2 n_position(float progress) const;
    vec2 N_tangent(float progress) const;
};

struct Idle{
    vec2 anchor;
    // using 2 different sine waves to make it seem more natural
    float xAmplitude, yAmplitude;
    float xFrequency, yFrequency; 

    Idle(vec2 anchor, float xAmplitude, float yAmplitude, float xFrequency, float yFrequency);

    vec2 n_position(float time) const;
    vec2 N_tangent(float time) const;
};

struct CatmullRomSpline{
    PointList<vec2> points;

    vec2 n_computePosition(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, float t) const;
    vec2 n_position(float t) const;
    vec2 N_tangent(float t) const;
};

struct Spiral{
    vec2 center;
    float startRadius, expansionSpeed, orbitSpeed;

    vec2 n_position(float t) const;
    vec2 N_tangent(float t) const;
};

struct Homing{
    float turnSpeed, moveSpeed, currentAngle;
    void update(vec2& enemyPos, const vec2& playerPos, float dt);
    vec2 N_tangent() const;
};

float lerpAngle(float a, float b, float t);

struct Movement{
    MOVEMENT_TYPE type;
    float timer = 0.0f;
    union{
        CubicBezier CB;
        Circle Circ;
        Idle Id;
        Homing Hom;
        CatmullRomSpline CRSpline;
        Spiral Spir;
        Dash D;
    };
};

template <typename T>
void chainedCubicBezierPath(PointList<T> pl){

}

void chainedCubicBezierPath();

#endif