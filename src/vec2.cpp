#include "vec2.hpp"


vec2f::vec2f()
{
    this->x = 0.f;
    this->y = 0.f;
}

vec2f::vec2f(float c)
{
    this->x = c;
    this->y = c;
}

vec2f::vec2f(float X, float Y)
{
    this->x = X;
    this->y = Y;
}

vec2f::vec2f(const vec2f& rhs)
{
    x = rhs.x;
    y = rhs.y;
}

vec2f vec2f::operator*(const float& c) const
{
   vec2f u; 
   u.x = c * x;
   u.y = c * y;

   return u;
}

vec2f operator*(const float& c, const vec2f& v)
{
   vec2f u; 
   u.x = c * v.x;
   u.y = c * v.y;

   return u;
}

vec2f vec2f::operator-(const vec2f& rhs) const
{
    vec2f v;
    v.x = this->x - rhs.x;
    v.y = this->y - rhs.y;

    return v;
}

vec2f vec2f::operator+(const vec2f& rhs) const
{
    vec2f v;
    v.x = this->x + rhs.x;
    v.y = this->y + rhs.y;

    return v;
}
