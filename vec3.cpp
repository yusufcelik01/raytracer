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

vec3f vec3f::operator-(const vec3f& rhs) const
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
    c.z = a.x * b.y - a.y * b.x;

    return c;
}

float dot(const vec3f& a, const vec3f& b)
{
    float c;
    
    c = a.x * b.x + a.y * b.y + a.z * b.z;

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

//vec3f operator*(const vec3f& v, float& c)
//{
//   vec3f u; 
//   u.x = c * v.x;
//   u.y = c * v.y;
//   u.z = c * v.z;
//
//   return u;
//}
vec3f vec3f::operator*(float& c)
{
   vec3f u; 
   u.x = c * this->x;
   u.y = c * this->y;
   u.z = c * this->z;

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


/// vec3i member functions
vec3i::vec3i()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}


vec3i::vec3i(vec3f vf)
{
    this->x = vf.x;
    this->y = vf.y;
    this->z = vf.z;
}

vec3i clamp(const vec3i a)
{
    vec3i v = a;
    if(v.x > 255){
        v.x = 255;
    }
    if(v.x < 0){
        v.x = 0;
    }
    if(v.y > 255){
        v.y = 255;
    }
    if(v.y < 0){
        v.y = 0;
    }
    if(v.z > 255){
        v.z = 255;
    }
    if(v.z < 0){
        v.z = 0;
    }
    return v;
}
