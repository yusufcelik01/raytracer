#ifndef __SPHERE__
#define __SPHERE__

#include "Object.hpp"

class Sphere: public Object
{
    public:
    int center_vertex_id;
    float radius;

    bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData);
    vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
    int getMaterialId();
};

#endif