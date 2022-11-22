#ifndef __AREA_LIGHT_DEFINED__
#define __AREA_LIGHT_DEFINED__

#include "vec3.hpp"
#include <cstddef>

class AreaLight
{
    public:
    vec3f position;
    vec3f normal;
    vec3f radiance;
    float extent;

    ~AreaLight();
    private:
    ONB* onb = NULL;
    public: 
    ONB getONB();
};



#endif
