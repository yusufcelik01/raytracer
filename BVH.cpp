#include "BVH.hpp"
#include "Face.hpp"
#include <algorithm>


bool BVH::intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData)
{
    if(bbox->hitRay(ray) == false)
    {
        return false;
    }
    
    IntersectionData hit1, hit2;

    bool hitLeft = left->intersectRay(VAO, ray, hit1);
    bool hitRight = right->intersectRay(VAO, ray, hit2);
    
    if(hitLeft)
    {
        intersectionData = hit1;
    }
    if(hitRight)
    {
        intersectionData = (hit2.t < hit1.t) ? hit2 : intersectionData; 
    }

    return (hitLeft || hitRight);
}

vec3f BVH::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const
{
    return vec3f(std::numeric_limits<float>::quiet_NaN());
}
int BVH::getMaterialId()
{
    return -1;
}



//template Object* constructBVH<Object*>(std::vector<vec3f, std::allocator<vec3f> > const&, std::vector<Object*, std::allocator<Object*> > &, AXIS_TYPE);
//template Object* constructBVH<Object*>(std::vector<vec3f, std::allocator<vec3f> > const&, std::vector<Object*, std::allocator<Object*> > const&, AXIS_TYPE);
