#ifndef __BOUNDING__VOLUME__HIERARCHY__DEFINED
#define __BOUNDING__VOLUME__HIERARCHY__DEFINED

#include "Object.hpp"
#include "BoundingBox.hpp"


class BVH : public Object
{
    public:
        Object* left;
        Object* right;

        BoundingBox bbox;
        bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData);
        vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
        int getMaterialId();

};

#endif
