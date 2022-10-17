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

vec3f::vec3f(const vec3i& ivec)
{
    this->x = ivec.x;
    this->y = ivec.y;
    this->z = ivec.z;
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

vec3f vec3f::operator+(const vec3f& rhs) const
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

/*
 all vectors must be unit
 */
vec3f reflect(vec3f n, vec3f incoming)
{
    return -incoming + ( 2.f * n * (dot(n, incoming)));
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
vec3f vec3f::operator*(const float& c) const
{
   vec3f u; 
   u.x = c * x;
   u.y = c * y;
   u.z = c * z;

   return u;
}
vec3f vec3f::operator/(const float& c) const
{
   vec3f u; 
   u.x = x / c;
   u.y = y / c;
   u.z = z / c;

   return u;
}
vec3f operator*(const float& c, const vec3f& v)
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
    this->x = int(vf.x);
    this->y = int(vf.y);
    this->z = int(vf.z);
}

vec3i clamp(const vec3i a, vec3i lower, vec3i upper)
{
    vec3i v = a;
    if(v.x > upper.x){
        v.x = upper.x;
    }
    else if(v.x < lower.x){
        v.x = lower.x;
    }
    
    if(v.y > upper.y){
        v.y = upper.y;
    }
    else if(v.y < lower.y){
        v.y = lower.y;
    }

    if(v.z > upper.z){
        v.z = upper.z;
    }
    else if(v.z < lower.z){
        v.z = lower.z;
    }
    return v;
}
vec3i clamp(const vec3i a, int lower, int upper)
{
    vec3i v = a;
    if(v.x > upper){
        v.x = upper;
    }
    else if(v.x < lower){
        v.x = lower;
    }
    
    if(v.y > upper){
        v.y = upper;
    }
    else if(v.y < lower){
        v.y = lower;
    }

    if(v.z > upper){
        v.z = upper;
    }
    else if(v.z < lower){
        v.z = lower;
    }
    return v;
}

vec3f vec3f::operator*(const vec3f& rhs) const
{
    vec3f ret;
    ret.x = x * rhs.x;
    ret.y = y * rhs.y;
    ret.z = z * rhs.z;
    return ret;
}
vec3f& vec3f::operator+=(const vec3f& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}
vec3f& vec3f::operator-=(const vec3f& rhs) 
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}
