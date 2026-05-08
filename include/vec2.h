#ifndef VEC2_H
#define VEC2_H

#include  "utilities.h"

using namespace usefulFunctions;

// this will mainly be used for movement to describe the direction and the speed at which the enemy/bullets are moving
class vec2{
    public:
        vec2();
        vec2(float x, float y);
        float x() const;
        float y() const;
        vec2 operator-() const;
        float operator[](int i) const;
        float& operator[](int i);
        const vec2& operator+=(const vec2& v);
        vec2& operator=(const vec2&v);
        vec2& operator*=(float t);
        vec2& operator/=(float t);
        float length() const;
        float lengthSquared() const;
        vec2 unitVector(const vec2& v);
        vec2 normalised();
        vec2 operator+(const vec2& other) const;

        static vec2 random(){
            vec2 r(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
            return r.unitVector(r);
        }

        static vec2 random(float min, float max){
            return vec2(random_float(min, max), random_float(min, max));
        }

        static vec2 polarToCartesian(float angle, float speed = 1.0f);
        bool nearZero();

    private:
        float e[2];
};

vec2 operator-(const vec2& u, const vec2& v);
vec2 operator*(const vec2& v, float t);
vec2 operator*(float t, const vec2& v);
vec2 operator/(const vec2& v, float t);

extern vec2 errorVector;

#endif