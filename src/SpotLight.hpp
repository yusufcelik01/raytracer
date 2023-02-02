#ifndef __SPOT_LIGHT_DEFINED__
#define __SPOT_LIGHT_DEFINED__

#include "vec3.hpp"

class SpotLight
{
    public:
    vec3f position;
    Spectrum intensity;
    vec3f direction;
    float covarageAngle, fallOffAngle;
};

#endif
