#ifndef MOVEMENT_GEOMETRY
#define MOVEMENT_GEOMETRY

#include "vec2.h"
#include "pointList.h"

// progress is in the [0,1] interval
// the n_position functions are for movement
// the N_tangent functions are for facing/firing direction

enum MOVEMENT_TYPE{NONE, CIRCLE, DASH, CUBIC_BEZIER, IDLE, SPIRAL, CATMULL_ROLL, HOMING};

struct CubicBezier{
    vec2 A; // A - start point of Bezier Curve 
    vec2 B; // end point of Bezier Curve
    vec2 C1, C2; // control points

    CubicBezier(vec2 A, vec2 B);
    CubicBezier(vec2 A, vec2 B, vec2 C);
    CubicBezier(vec2 A, vec2 B, vec2 C1, vec2 C2);
    ~CubicBezier(){}

    vec2 n_position(float progress) const;
    vec2 N_tangent(float progress) const;
};

struct Circle{
    vec2 center;
    float radius;

    Circle(vec2 center, float radius);
    vec2 n_position(float progress) const;
    vec2 N_tangent(float progress) const;
    ~Circle(){}
};

struct Dash{
    vec2 start, end;

    Dash(vec2 start, vec2 end);

    vec2 n_position(float progress) const;
    vec2 N_tangent(float progress) const;

    ~Dash(){}
};

struct Idle{
    vec2 anchor;
    // using 2 different sine waves to make it seem more natural
    float xAmplitude, yAmplitude;
    float xFrequency, yFrequency; 

    Idle(vec2 anchor, float xAmplitude, float yAmplitude, float xFrequency, float yFrequency);

    vec2 n_position(float time) const;
    vec2 N_tangent(float time) const;

    ~Idle(){}
};

struct CatmullRomSpline{
    PointList<vec2> points;

    vec2 n_computePosition(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, float t) const;
    vec2 n_position(float t) const;
    vec2 N_tangent(float t) const;

    CatmullRomSpline(){
        points.head = nullptr;
        points.size = 0;
    }

    ~CatmullRomSpline(){
        points.clean();
    }
};

struct Spiral{
    vec2 center;
    float startRadius, expansionSpeed, orbitSpeed;

    vec2 n_position(float t) const;
    vec2 N_tangent(float t) const;

    Spiral(vec2 center, float startRadius, float expansionSpeed, float orbitSpeed);
    ~Spiral(){}
};

struct Homing{
    float turnSpeed, moveSpeed, currentAngle;
    void update(vec2& enemyPos, const vec2& playerPos, float dt);
    vec2 N_tangent() const;

    Homing(float turnSpeed, float moveSpeed, float currentAngle);
    ~Homing(){}
};

float lerpAngle(float a, float b, float t);

struct Movement{
    MOVEMENT_TYPE type;
    float timer = 0.0f;
    float duration;

    union GEOMETRY{
        CubicBezier CB;
        Circle Circ;
        Idle Id;
        Homing Hom;
        CatmullRomSpline CRSpline;
        Spiral Spir;
        Dash D;

        GEOMETRY(){}
        ~GEOMETRY(){}
    } geometry;

    void set(const CubicBezier& cb);
    void set(const Circle& circle);
    void set(const Idle& idle);
    void set(const Homing& homing);
    void setCatmullRomSpline();
    void set(const Spiral& spiral);
    void set(const Dash& dash);
    void destroy();

    Movement() : type(MOVEMENT_TYPE::NONE) {}
    ~Movement(){
        destroy();
    }
    Movement& operator=(const Movement& other);
    
};

// it is used as a vector which describes all the movements the enemy/bullet can do
struct MovementState {
    Movement mv;
    float t         = 0.0f;
    float totalTime = 0.0f;
    bool isFinished = false;

    MovementState() = default;
    MovementState(const MovementState& other); // wrote this one like a moron when the compiler complained about me not having the one from bellow, still keeping this 'cause maybe I'll need it and it's already written
    MovementState& operator=(const MovementState& other);
};

#endif