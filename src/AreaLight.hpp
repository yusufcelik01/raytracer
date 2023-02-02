#ifndef __AREA_LIGHT_DEFINED__
#define __AREA_LIGHT_DEFINED__

#include "vec3.hpp"
#include "UniformRandomGenerator.hpp"
#include <cstddef>
#include "Spectrum.hpp"

class AreaLight
{
    public:
    vec3f position;
    vec3f normal;
    Spectrum radiance;
    float extent;
    UniformRandomGenerator rng;

    ~AreaLight();
    private:
    ONB* onb = NULL;
    public: 
    ONB getONB();
    vec3f getLightSample();
};



#endif
