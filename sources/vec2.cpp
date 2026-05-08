#include "../include/vec2.h"

vec2::vec2(){
    e[0] = 0;
    e[1] = 0;
}

vec2::vec2(float x, float y){
    e[0] = x;
    e[1] = y;
}

float vec2::x() const {return e[0];}

float vec2::y() const {return e[1];}

vec2 vec2::operator-() const{ return vec2(-e[0], -e[1]);}

float vec2::operator[](int i) const{ return e[i];}

float& vec2::operator[](int i) { return e[i];}

const vec2& vec2::operator+=(const vec2& v){
    e[0] += v.e[0];
    e[1] += v.e[1];
    return *this;
}

vec2& vec2::operator=(const vec2&v){
    e[0] = v.e[0];
    e[1] = v.e[1];
    return *this;
}

vec2& vec2::operator*=(float t){
    e[0] *= t;
    e[1] *= t;
    return *this;
}

vec2& vec2::operator/=(float t){
    return *this *= (1/t);
}

vec2 vec2::operator+(const vec2& other) const {
    return vec2(x() + other.x(), y() + other.y());
}

float vec2::length() const{
    return SDL_sqrt(lengthSquared());
}

float vec2::lengthSquared() const{
    return e[0]*e[0] + e[1]*e[1];
}

vec2 vec2::normalised(){
    return unitVector(*this);
}

bool vec2::nearZero(){
    double s = 1e-8;
    return (std::fabs(e[0]) < s && std::fabs(e[1]) < s);
}

//Vector Utility Functions
std::ostream& operator<<(std::ostream& out, const vec2& v){
    return out << v.x() << ' ' << v.y(); 
}

vec2 operator+(const vec2& u, const vec2& v){
    return vec2(u.x() + v.x(), u.y() + v.y());
}

vec2 operator-(const vec2& u, const vec2& v){
    return vec2(u.x() - v.x(), u.y() - v.y());
}

vec2 operator*(const vec2& v, float t){
    return vec2(v.x() * t, v.y() * t);
}

vec2 operator*(float t, const vec2& v){
    return v*t;
}

vec2 operator/(const vec2& v, float t){
    return v * (1/t);
}

float dot(const vec2& u, const vec2& v){
    return u.x() * v.x() + u.y() * v.y();
}

vec2 vec2::unitVector(const vec2& v){
    return v / v.length();
}

vec2 randomUnitVector(){
    while(true){
        auto p = vec2::random(-1, 1);
        auto lensq = p.lengthSquared();
        if(1e-160 < lensq && lensq <= 1)
        return p / sqrt(lensq);
    }
}

vec2 vec2::polarToCartesian(float angle, float speed){
    float radians = angle * M_PI / 180.0f;
    return vec2(SDL_cos(radians) * speed, SDL_sin(radians) * speed);
}

vec2 errorVector = vec2(-793,-793);