#include "Triangle.hpp"
#include <cstddef>

Triangle::Triangle()
{
    indices = Face();
    material_id = -1;
}

Triangle::Triangle(const Triangle& rhs)
{
    indices = rhs.indices;
    material_id = rhs.material_id;
}

Triangle::~Triangle()
{
    if(bbox)
    {
        delete bbox;        
    }
    indices.~Face();
}

bool Triangle::intersectRay(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData) 
{
    if(this->indices.intersectRay(VAO, r, intData))
    {
        //intData.obj = this;
        intData.hitType = TRIANGLE;
        intData.material_id = this->material_id;
        return true;
    }
    else
    {
        return false;
    }
}

//vec3f Triangle::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const
//{
//    //TODO dummy 
//    //Triangle* triangle = intersectionPoint.obj;
//    vec3f a = VAO[this->indices.v0_id - 1];
//    vec3f b = VAO[this->indices.v1_id - 1];
//    vec3f c = VAO[this->indices.v2_id - 1];
//
//    return norm(cross(b-a, c-b));
//}

int Triangle::getMaterialId()
{
    return this->material_id;
}

BoundingBox* Triangle::getBoundingBox(const std::vector<vec3f>& VAO)
{
    if(bbox != NULL)
    {
        return bbox;
    }
    bbox = indices.getBoundingBox(VAO);     
    return bbox;
}

BoundingBox* Triangle::getBoundingBox() const
{
    return bbox;
}



