#ifndef __OBJECT__
#define __OBJECT__

#include <vector>
#include <limits>
#include <cstddef>
#include "Ray.hpp"
#include "IntersectionData.hpp"
#include "BoundingBox.hpp"
#include "Texture.hpp"
#include "rtmath.hpp"
#include "VertexBuffers.hpp"


class Object
{
    public:
        int material_id;
        Material material;
        BoundingBox* bbox = NULL;
        mat4x4* transformation = NULL;
        vec3f* motionBlur = NULL;

        std::vector<Texture*> textures;

        //member functions
        Object();
        Object(const Object& rhs);
        ~Object();

        //virtual bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData) = 0;
        virtual bool intersectRay(const VertexBuffers& vertexBuffers, const Ray& ray, IntersectionData& intersectionData) = 0;
        //virtual vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const = 0;
        virtual BoundingBox* getBoundingBox(const std::vector<vec3f>& VAO) = 0;
        virtual BoundingBox* getBoundingBox() const = 0;
        virtual int getMaterialId() = 0;
};

#endif
