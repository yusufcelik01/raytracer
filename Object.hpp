#ifndef __OBJECT__
#define __OBJECT__

#include "Ray.hpp"
#include "rtmath.hpp"
#include <vector>

class IntersectionData
{
    public:
    float t;
    int faceId;//if it is a mesh
};

class Object
{
    public:
        bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData);
    
        vec3f getSurfNormal(const IntersectionData& intersectionPoint);
        int getMaterialId();
};

#endif
