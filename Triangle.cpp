#include "Triangle.hpp"

bool Triangle::intersectRay(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData) 
{
    if(this->indices.intersectRay(VAO, r, intData))
    {
        //intData.obj = this;
        intData.material_id = this->material_id;
        return true;
    }
    else
    {
        return false;
    }
}

vec3f Triangle::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const
{
    //TODO dummy 
    //Triangle* triangle = intersectionPoint.obj;
    vec3f a = VAO[this->indices.v0_id - 1];
    vec3f b = VAO[this->indices.v1_id - 1];
    vec3f c = VAO[this->indices.v2_id - 1];

    return norm(cross(b-a, c-b));
}

int Triangle::getMaterialId()
{
    return this->material_id;
}
