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
