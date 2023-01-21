#ifndef __DIRECTIONAL_LIGHT_DEFINED__
#define __DIRECTIONAL_LIGHT_DEFINED__

#include "vec3.hpp"

class DirectionalLight
{
    public:
    vec3f direction;
    vec3f radiance;
};

#endif

