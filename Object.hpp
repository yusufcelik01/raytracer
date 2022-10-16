#ifndef __OBJECT__
#define __OBJECT__

#include "Ray.hpp"
#include "rtmath.hpp"


class Object
{
    public:
        bool intersectRay(const Ray& ray, float& t);
    
        vec3f getSurfNormal(vec3f point);
        int getMaterialId();
};

#endif
