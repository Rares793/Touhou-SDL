#ifndef VEC2_H
#define VEC2_H

class vec2{
    public:
        vec2();
        vec2(float x, float y);
        float x() {return e[0];}
        float y() {return e[1];}
        vec2 operator-() const{ return vec2(-e[0], -e[1]);}
        float operator[](int i) const{ return e[i];}
        float& operator[](int i) { return e[i];}
        const vec2& operator+=(const vec2& v){
            e[0] = v.e[0];
            e[1] = v.e[1];
            return *this;
        }

        vec2& operator=(const vec2&v){
            e[0] = v.e[0];
            e[1] = v.e[1];
            return *this;
        }

        vec2& operator*=(float t){
            e[0] *= t;
            e[1] *= t;
            return *this;
        }

        

    private:
        float e[2];
};
#endif