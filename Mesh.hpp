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
        
        bool isInstanced;
        Mesh* baseMesh;
        bool resetTransform;

        Mesh();
        Mesh(const Mesh& rhs);
        ~Mesh();

        //member functions
        //vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
        bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intData);
        bool intersectRayResetTransform(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intData);
        int getMaterialId();
        BoundingBox* getBoundingBox(const std::vector<vec3f>& VAO);
        BoundingBox* getBoundingBox() const;

};


#endif
