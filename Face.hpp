#ifndef __FACE__
#define __FACE__

#include "Object.hpp"

class Face : public Object
{
    public:
    int v0_id;
    int v1_id;
    int v2_id;
    
    //constructors
    Face();
    Face(const Face& rhs);
    Face(size_t v0, size_t v1, size_t v2);
    ~Face();

    //member functions
    //bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData);
    bool intersectRay(const VertexBuffers& vertexBuffers, const Ray& ray, IntersectionData& intersectionData);
    vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
    int getMaterialId();
    virtual BoundingBox* getBoundingBox(const std::vector<vec3f>& VAO);
    virtual BoundingBox* getBoundingBox() const;
};


#endif
