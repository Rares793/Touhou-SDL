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
    if(points.size < 4)
        return errorVector;

    int segmentCount = points.size - 3;
    float scaled = t * segmentCount;
    int i = (int) scaled;
    float local = scaled - i;

    // guard against t == 1.0 pushing i out of bounds
    if(i >= segmentCount) i = segmentCount - 1;

    Node<vec2>* n0 = points.getNodeAtPosition(i);
    Node<vec2>* n1 = points.getNodeAtPosition(i + 1);
    Node<vec2>* n2 = points.getNodeAtPosition(i + 2);
    Node<vec2>* n3 = points.getNodeAtPosition(i + 3);

    if(!n0 || !n1 || !n2 || !n3)
        return errorVector;

    return n_computePosition(n0->v, n1->v, n2->v, n3->v, local);
}

vec2 CatmullRomSpline::N_tangent(float t) const{
    float h = 0.001f;
   return (n_position(std::min(t+h, 1.0f)) - n_position(std::max(t-h, 0.0f))).normalised();
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

Spiral::Spiral(vec2 center,float startRadius, float expansionSpeed, float orbitSpeed){
    this->center = center;
    this->startRadius = startRadius;
    this->expansionSpeed = expansionSpeed;
    this->orbitSpeed = orbitSpeed;
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

Homing::Homing(float turnSpeed, float moveSpeed, float currentAngle){
    this->turnSpeed = turnSpeed;
    this->moveSpeed = moveSpeed;
    this->currentAngle = currentAngle;
}

// Movement

void Movement::set(const CubicBezier& cb){
    destroy();
    this->type = MOVEMENT_TYPE::CUBIC_BEZIER;
    new (&geometry.CB) CubicBezier(cb.A, cb.B, cb.C1, cb.C2);
    type = MOVEMENT_TYPE::CUBIC_BEZIER;
}

void Movement::set(const Circle& circle){
    destroy();
    this->type = MOVEMENT_TYPE::CIRCLE;
    new (&geometry.Circ) Circle(circle.center, circle.radius);
    type = MOVEMENT_TYPE::CIRCLE;
}

void Movement::set(const Idle& idle){
    destroy();
    this->type = MOVEMENT_TYPE::IDLE;
    new (&geometry.Id) Idle(idle.anchor, idle.xAmplitude, idle.yAmplitude, idle.xFrequency, idle.yFrequency);
    type = MOVEMENT_TYPE::IDLE;
}

void Movement::set(const Homing& homing){
    destroy();
    this->type = MOVEMENT_TYPE::HOMING;
    new (&geometry.Hom) Homing(homing.turnSpeed, homing.moveSpeed, homing.currentAngle);
    type = MOVEMENT_TYPE::HOMING;
}

void Movement::setCatmullRomSpline(){
    destroy();
    this->type = MOVEMENT_TYPE::CATMULL_ROLL;
    new (&geometry.CRSpline) CatmullRomSpline();
    type = MOVEMENT_TYPE::CATMULL_ROLL;
}

void Movement::set(const Spiral& spiral){
    destroy();
    this->type = MOVEMENT_TYPE::SPIRAL;
    new (&geometry.Spir) Spiral(spiral.center, spiral.startRadius, spiral.expansionSpeed, spiral.orbitSpeed);
    type = MOVEMENT_TYPE::SPIRAL;
}

void Movement::set(const Dash& dash){
    destroy();
    this->type = MOVEMENT_TYPE::DASH;
    new (&geometry.D) Dash(dash.start, dash.end);
    type = MOVEMENT_TYPE::DASH;
}

void Movement::destroy(){
    switch(type){
        case MOVEMENT_TYPE::CUBIC_BEZIER:
            geometry.CB.~CubicBezier();
            break;
        case MOVEMENT_TYPE::CATMULL_ROLL:
            geometry.CRSpline.~CatmullRomSpline();
            break;
        case MOVEMENT_TYPE::CIRCLE:
            geometry.Circ.~Circle();
            break;
        case MOVEMENT_TYPE::DASH:
            geometry.D.~Dash();
            break;
        case MOVEMENT_TYPE::HOMING:
            geometry.Hom.~Homing();
            break;
        case MOVEMENT_TYPE::IDLE:
            geometry.Id.~Idle();
            break;
        case MOVEMENT_TYPE::SPIRAL:
            geometry.Spir.~Spiral();
            break;
        case MOVEMENT_TYPE::NONE:
            break; // man, I despise this so, so, so much. I hope I never come up with such dogshit code ever again
        default:
            std::cout << "Case " << type << " not handled by Movement::destroy()!\n";

    }
}

Movement& Movement::operator=(const Movement& other){
    if(this == &other) return *this;
    destroy();
    switch(other.type){
        case MOVEMENT_TYPE::CUBIC_BEZIER: set(other.geometry.CB);   break;
        case MOVEMENT_TYPE::CIRCLE:       set(other.geometry.Circ); break;
        case MOVEMENT_TYPE::DASH:         set(other.geometry.D);    break;
        case MOVEMENT_TYPE::IDLE:         set(other.geometry.Id);   break;
        case MOVEMENT_TYPE::HOMING:       set(other.geometry.Hom);  break;
        case MOVEMENT_TYPE::SPIRAL:       set(other.geometry.Spir); break;
        case MOVEMENT_TYPE::CATMULL_ROLL: {
            setCatmullRomSpline();
            Node<vec2>* current = other.geometry.CRSpline.points.head;
            while(current){
                geometry.CRSpline.points.push(current->v);
                current = current->next;
            }
            break;
        }
        case MOVEMENT_TYPE::NONE: break;
    }
    return *this;
}

// Movement State

MovementState::MovementState(const MovementState& other) {
    t         = other.t;
    totalTime = other.totalTime;
    switch(other.mv.type){
        case MOVEMENT_TYPE::CUBIC_BEZIER: 
            mv.set(other.mv.geometry.CB);   
            break;
        case MOVEMENT_TYPE::CIRCLE:
           mv.set(other.mv.geometry.Circ); 
           break;
        case MOVEMENT_TYPE::DASH:         
            mv.set(other.mv.geometry.D);    
            break;
        case MOVEMENT_TYPE::IDLE:         
            mv.set(other.mv.geometry.Id);   
            break;
        case MOVEMENT_TYPE::HOMING:       
            mv.set(other.mv.geometry.Hom);  
            break;
        case MOVEMENT_TYPE::SPIRAL:       
            mv.set(other.mv.geometry.Spir); 
            break;
        case MOVEMENT_TYPE::CATMULL_ROLL:{
            mv.setCatmullRomSpline();
            Node<vec2>* current = other.mv.geometry.CRSpline.points.head;
            while(current){
                mv.geometry.CRSpline.points.push(current->v);
                current = current->next;
            }
            break;
        }
        case MOVEMENT_TYPE::NONE: break;
    }
}

MovementState& MovementState::operator=(const MovementState& other){
    if(this == &other) return *this;
    mv        = other.mv;
    t         = other.t;
    totalTime = other.totalTime;
    return *this;
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