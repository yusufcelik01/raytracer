#include "BVH.hpp"
#include "Face.hpp"
#include <algorithm>
#include <iostream>


BVH::BVH()
{
    left = right = NULL;
    bbox = NULL;
}

BVH::~BVH()
{
    if(left){
        delete left;
    }
    if(right){
        delete right;
    }
}


//bool BVH::intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intersectionData)
bool BVH::intersectRay(const VertexBuffers& vertexBuffers, const Ray& ray, IntersectionData& intersectionData)
{
    //const std::vector<vec3f>& VAO = vertexBuffers.vertexCoords;
    if(bbox->hitRay(ray) == false)
    {
        return false;
        //std::cout << "RAY MISSED BVH" << std::endl;
    }
    //std::cout << "RAY HIT BVH" << std::endl;
    
    IntersectionData hit1, hit2;
    hit1.t = FLOAT_MAX;
    hit2.t = FLOAT_MAX;

    bool hitLeft = false;
    bool hitRight = false;
    if(left) {
        //std::cout << "BVH has left" << std::endl;
        hitLeft = left->intersectRay(vertexBuffers, ray, hit1);
    }
    if(right) {
        //std::cout << "BVH has right" << std::endl;
        hitRight = right->intersectRay(vertexBuffers, ray, hit2);
    }
    
    if(hitLeft)
    {
        //std::cout << "RAY HIT BVH Left" << std::endl;
        intersectionData = hit1;
    }
    if(hitRight)
    {
        //std::cout << "RAY HIT BVH Right" << std::endl;
        intersectionData = (hit2.t < hit1.t) ? hit2 : intersectionData; 
    }

    return (hitLeft || hitRight);
}

//vec3f BVH::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const
//{
//    return vec3f(0.f);
//    //return vec3f(std::numeric_limits<float>::quiet_NaN());
//}
int BVH::getMaterialId()
{
    return -1;
}
BoundingBox* BVH::getBoundingBox(const std::vector<vec3f>& VAO)
{
    return bbox;
}
BoundingBox* BVH::getBoundingBox() const
{
    return bbox;
}



//template Object* constructBVH<Object*>(std::vector<vec3f, std::allocator<vec3f> > const&, std::vector<Object*, std::allocator<Object*> > &, AXIS_TYPE);
//template Object* constructBVH<Object*>(std::vector<vec3f, std::allocator<vec3f> > const&, std::vector<Object*, std::allocator<Object*> > const&, AXIS_TYPE);
