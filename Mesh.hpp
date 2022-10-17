#ifndef __MESH__
#define __MESH__

#include "Object.hpp"
#include "Face.hpp"
#include <vector>


class Mesh :public Object
{
    public:
    std::vector<Face> faces;

    //member functions
    bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData);
    vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
    int getMaterialId();

};


#endif
