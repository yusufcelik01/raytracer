#ifndef __RAY__
#define __RAY__

#include "vec3.hpp"
#include "vec2.hpp"

enum RayType
{
    RAY_TYPE_PRIMARY_RAY,
    RAY_TYPE_SHADOW_RAY,
    RAY_TYPE_RECURSING_RAY,
    RAY_TYPE_GLOBAL_ILLUMINATION_RAY,
};

class Ray
{
    public:
    vec3f o;
    vec3f d;
    float time = 0.f;
    vec2f texCoord;
    RayType type = RAY_TYPE_RECURSING_RAY;
    int depth; 
    float throughput;
};

#endif

