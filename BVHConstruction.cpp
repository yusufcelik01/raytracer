#include "BVHConstruction.hpp"
#include <algorithm>


bool ObjectCmpBBoxx(const Object* a, const Object* b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->xmin + bboxA->xmax)/2.f;
    float centerB = (bboxB->xmin + bboxB->xmax)/2.f;
    return (centerA < centerB);
}
bool ObjectCmpBBoxy(const Object*& a, const Object*& b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->ymin + bboxA->ymax)/2.f;
    float centerB = (bboxB->ymin + bboxB->ymax)/2.f;
    return (centerA < centerB);
}

bool ObjectCmpBBoxz(const Object*& a, const Object*& b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->zmin + bboxA->zmax)/2.f;
    float centerB = (bboxB->zmin + bboxB->zmax)/2.f;
    return (centerA < centerB);
}

bool FaceCmpBBoxx( Face* a,  Face* b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->xmin + bboxA->xmax)/2.f;
    float centerB = (bboxB->xmin + bboxB->xmax)/2.f;
    return (centerA < centerB);
}
bool FaceCmpBBoxy(const Face*& a, const Face*& b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->ymin + bboxA->ymax)/2.f;
    float centerB = (bboxB->ymin + bboxB->ymax)/2.f;
    return (centerA < centerB);
}

bool FaceCmpBBoxz(const Face*& a, const Face*& b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->zmin + bboxA->zmax)/2.f;
    float centerB = (bboxB->zmin + bboxB->zmax)/2.f;
    return (centerA < centerB);
}



Object* constructObjectBVH(const std::vector<vec3f>& VAO, std::vector<Object*>& objects, AXIS_TYPE axis)
{
    auto end = objects.end();
    auto begin = objects.begin();

    size_t size = end - begin;

    if(size == 0)
    {
        return NULL;
    }
    if(size == 1)
    {
        return *begin;
    }

    //force all the objects to calculate their bboxes 
    //so that we dont have to compute bboxes for each comparison

    //sort all objects and choose median
    switch(axis)
    {
        case X_AXIS:
            std::sort(begin, end, ObjectCmpBBoxx);         
            break;
        case Y_AXIS:
            //std::sort(begin, end, cmpBBoxy);         
            break;
        case Z_AXIS:
            //std::sort(begin, end, cmpBBoxz);         
            break;
    }


    return NULL;
}

Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>& objects, AXIS_TYPE axis)
{
    auto end = objects.end();
    auto begin = objects.begin();

    size_t size = end - begin;

    if(size == 0)
    {
        return NULL;
    }
    if(size == 1)
    {
        return *begin;
    }

    //force all the objects to calculate their bboxes 
    //so that we dont have to compute bboxes for each comparison

    //sort all objects and choose median
    switch(axis)
    {
        case X_AXIS:
            std::sort(begin, end, FaceCmpBBoxx);         
            break;
        case Y_AXIS:
            //std::sort(begin, end, cmpBBoxy);         
            break;
        case Z_AXIS:
            //std::sort(begin, end, cmpBBoxz);         
            break;
    }


    return NULL;
}

