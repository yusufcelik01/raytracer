#ifndef __OBJECT__
#define __OBJECT__

#include "Ray.hpp"
#include "IntersectionData.hpp"
#include "rtmath.hpp"
#include <vector>
#include <limits>



class Object
{
    public:
        int material_id;
        virtual bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData) = 0;

        virtual vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const = 0;
        virtual int getMaterialId() = 0;
};

#endif
