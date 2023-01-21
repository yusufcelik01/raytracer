#include "vec4.hpp"

vec4f::vec4f()
{
    x = y = z = w = 0.f;
}

vec4f::vec4f(float value)
{
    x = y = z = w = value;
}

vec4f::vec4f(float X, float Y, float Z, float W)
{
    x = X;
    y = Y;
    z = Z;
    w = W;
}

vec4f::vec4f(vec3f v, float W)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = W;
}
