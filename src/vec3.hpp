#ifndef __VEC3__DEFINED__
#define __VEC3__DEFINED__
#include <ostream>

struct vec3i;

struct vec3f
{
    public:
    union
    {
        struct{float x, y, z;};
        struct{float r, g, b;};
        struct{float s, t, p;};
    };

    vec3f();
    vec3f(float);
    vec3f(float, float, float);
    vec3f(const vec3i& ivec);
    //vec3f(const vec3f& vec);
    vec3f operator-();
    vec3f operator-(const vec3f& rhs) const;
    vec3f operator+(const vec3f& rhs) const;
    vec3f operator*(const float& c) const;
    vec3f operator/(const float& c) const;
    vec3f operator*(const vec3f& rhs) const;
    vec3f& operator+=(const vec3f& rhs) ;
    vec3f& operator-=(const vec3f& rhs) ;
    vec3f& operator/=(const float c) ;
    bool operator==(const vec3f& rhs) ;
    float& operator[](int index);
};

struct vec3i
{
    public:
    union
    {
        struct{int x, y, z;};
        struct{int r, g, b;};
        struct{int s, t, p;};
    };

    vec3i();
    vec3i(vec3f vf);
};

//orthonormal basis
class ONB
{
    public:
    vec3f n, u, v;

    ONB();
    ONB(vec3f); 
};


vec3f cross(const vec3f& a, const vec3f& b);
float dot(const vec3f& a, const vec3f& b);
float length(const vec3f& a);
vec3f norm(const vec3f& a);
vec3i clamp(const vec3i a, int, int);
vec3i clamp(const vec3i a, vec3i, vec3i);
vec3f reflect(vec3f n, vec3f incoming);
vec3f refract(vec3f n, vec3f incoming, float eta);
float dielectricReflectionRatio(float n1, float n2, float cosTheta, float cosPhi);
float conductorReflectionRatio(float n2, float k2, float cosTheta);
vec3f exp(vec3f v);
vec3f elemWiseExp(vec3f v, float);
float makeGrayScale(vec3f rgb);
float getMaxComp(vec3f v);

vec3f operator*(const float& c, const vec3f& v);
std::ostream& operator<<(std::ostream& os, const vec3f& v);

#endif
