#ifndef __SPHERICAL_ENVIRONMENT_LIGHT_DEFINED__
#define __SPHERICAL_ENVIRONMENT_LIGHT_DEFINED__

#include "ImageTexture.hpp"

class SphericalEnvLight
{
    public:
    ImageTexture tex;
    vec3f sample(vec3f dir);//TODO replace with spectrum
};

#endif
