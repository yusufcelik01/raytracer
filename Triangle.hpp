#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "Face.hpp"
#include "Object.hpp"

class Triangle: public Object
{
    public:
    Face indices;

    //constructors
    Triangle();
    Triangle(const Triangle& rhs);
    ~Triangle();

    //member functions
    bool intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData) ;
    //vec3f getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const;
    int getMaterialId();
    virtual BoundingBox* getBoundingBox(const std::vector<vec3f>& VAO);
    virtual BoundingBox* getBoundingBox() const;
};

#endif
