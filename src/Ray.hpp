#ifndef __RAY__
#define __RAY__

#include "vec3.hpp"
#include "vec2.hpp"

class Ray
{
    public:
    vec3f o;
    vec3f d;
    float time = 0.f;
    vec2f texCoord;
};

#endif

