#include "vec3.hpp"
#include <cmath>


vec3f::vec3f()
{
    this->x = 0.f;
    this->y = 0.f;
    this->z = 0.f;
}

vec3f::vec3f(float c)
{
    this->x = c;
    this->y = c;
    this->z = c;
}

vec3f::vec3f(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3f vec3f::operator-()
{
    vec3f u;
    u.x = -this->x;
    u.y = -this->y;
    u.z = -this->z;

    return u;
}

vec3f vec3f::operator-(const vec3f& rhs)
{
    vec3f v;
    v.x = this->x - rhs.x;
    v.y = this->y - rhs.y;
    v.z = this->z - rhs.z;

    return v;
}

vec3f vec3f::operator+(const vec3f& rhs)
{
    vec3f v;
    v.x = this->x + rhs.x;
    v.y = this->y + rhs.y;
    v.z = this->z + rhs.z;

    return v;
}

vec3f cross(const vec3f& a, const vec3f& b)
{
    vec3f c;
    
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.z;

    return c;
}

vec3f dot(const vec3f& a, const vec3f& b)
{
    vec3f c;
    
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;

    return c;
}

float length(const vec3f& a)
{
    float l = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);

    return l;
}

vec3f norm(const vec3f& a)
{
    float k = 1.f/sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
     

    return k * a;
}

vec3f operator*(const vec3f& v, float& c)
{
   vec3f u; 
   u.x = c * v.x;
   u.y = c * v.y;
   u.z = c * v.z;

   return u;
}
vec3f operator*(float& c, const vec3f& v)
{
   vec3f u; 
   u.x = c * v.x;
   u.y = c * v.y;
   u.z = c * v.z;

   return u;
}
