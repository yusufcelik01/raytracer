#ifndef __MESH__
#define __MESH__

#include "Object.hpp"
#include "Face.hpp"
#include "BVH.hpp"
#include <vector>


class Mesh :public Object
{
    public:
        Object* AccBVH = NULL;
        std::vector<Face*> faces;
        mat4x4* transformation = NULL;

        Mesh();
        Mesh(const Mesh& rhs);
        ~Mesh();

        //member functions
        bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData);
        vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
        int getMaterialId();
        BoundingBox* getBoundingBox(const std::vector<vec3f>& VAO);
        BoundingBox* getBoundingBox() const;

};


#endif
