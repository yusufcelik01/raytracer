#ifndef __OBJECT__
#define __OBJECT__

#include <vector>
#include <limits>
#include <cstddef>
#include "Ray.hpp"
#include "IntersectionData.hpp"
#include "BoundingBox.hpp"
#include "rtmath.hpp"



class Object
{
    public:
        int material_id;
        BoundingBox* bbox = NULL;
        mat4x4* transformation = NULL;
        mat4x4* motionBlur = NULL;

        //member functions
        virtual bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData) = 0;
        //virtual vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const = 0;
        virtual BoundingBox* getBoundingBox(const std::vector<vec3f>& VAO) = 0;
        virtual BoundingBox* getBoundingBox() const = 0;
        virtual int getMaterialId() = 0;
};

#endif
