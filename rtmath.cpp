#include "rtmath.hpp"

template <class T>
T mix(T x, T y, float a)
{
    return x * (1 - a) + y * a;
}

float clamp(float x, float min, float max)
{
    x = min > x ? min : x;
    x = max < x ? max : x;
    return x;
}

float clamp(float x, float min, float max);


template vec3f mix<vec3f>(vec3f, vec3f, float);
template float mix<float>(float, float, float);
