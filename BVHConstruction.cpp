#include "BVHConstruction.hpp"
#include <algorithm>
#include <iostream>



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
bool ObjectCmpBBoxy(Object*& a, Object*& b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->ymin + bboxA->ymax)/2.f;
    float centerB = (bboxB->ymin + bboxB->ymax)/2.f;
    return (centerA < centerB);
}

bool ObjectCmpBBoxz(Object*& a, Object*& b)
{
    BoundingBox* bboxA;
    BoundingBox* bboxB;

    bboxA = a->getBoundingBox();
    bboxB = b->getBoundingBox();

    float centerA = (bboxA->zmin + bboxA->zmax)/2.f;
    float centerB = (bboxB->zmin + bboxB->zmax)/2.f;
    return (centerA < centerB);
}

struct FaceCmpBBoxx
{
    bool const operator()(Face* a, Face* b) const
    {

        BoundingBox* bboxA;
        BoundingBox* bboxB;

        bboxA = a->getBoundingBox();
        bboxB = b->getBoundingBox();

        float centerA = (bboxA->xmin + bboxA->xmax)/2.f;
        float centerB = (bboxB->xmin + bboxB->xmax)/2.f;

        return (centerA < centerB);
    }
};

struct FaceCmpBBoxy
{
    bool const operator() (Face* a, Face* b) const
    {
        BoundingBox* bboxA;
        BoundingBox* bboxB;

        bboxA = a->getBoundingBox();
        bboxB = b->getBoundingBox();

        float centerA = (bboxA->ymin + bboxA->ymax)/2.f;
        float centerB = (bboxB->ymin + bboxB->ymax)/2.f;
        return (centerA < centerB);
    }
};

struct FaceCmpBBoxz
{
    bool const operator() (Face* a, Face* b) const
    {
        BoundingBox* bboxA;
        BoundingBox* bboxB;

        bboxA = a->getBoundingBox();
        bboxB = b->getBoundingBox();

        float centerA = (bboxA->zmin + bboxA->zmax)/2.f;
        float centerB = (bboxB->zmin + bboxB->zmax)/2.f;
        return (centerA < centerB);
    }
};


template <typename T>
struct cmpBBoxx
{
    bool const operator()(T* a, T* b) const
    {

        BoundingBox* bboxA;
        BoundingBox* bboxB;

        bboxA = a->getBoundingBox();
        bboxB = b->getBoundingBox();

        float centerA = (bboxA->xmin + bboxA->xmax);
        float centerB = (bboxB->xmin + bboxB->xmax);

        return (centerA < centerB);
    }
};

template <typename T>
struct cmpBBoxy
{
    bool const operator() (T* a, T* b) const
    {
        BoundingBox* bboxA;
        BoundingBox* bboxB;

        bboxA = a->getBoundingBox();
        bboxB = b->getBoundingBox();

        float centerA = (bboxA->ymin + bboxA->ymax);
        float centerB = (bboxB->ymin + bboxB->ymax);
        return (centerA < centerB);
    }
};

template <typename T>
struct cmpBBoxz
{
    bool const operator() (T* a, T* b) const
    {
        BoundingBox* bboxA;
        BoundingBox* bboxB;

        bboxA = a->getBoundingBox();
        bboxB = b->getBoundingBox();

        float centerA = (bboxA->zmin + bboxA->zmax);
        float centerB = (bboxB->zmin + bboxB->zmax);
        return (centerA < centerB);
    }
};


float getBBcenter(const Face& obj, AXIS_TYPE axis)
{
    BoundingBox* box = obj.getBoundingBox();

    float mid; 
    switch(axis)
    {
        case X_AXIS:
            mid = box->xmin + box->xmax;
            break;
        case Y_AXIS:
            mid = box->ymin + box->ymax;
            break;
        case Z_AXIS:
            mid = box->zmin + box->zmax;
            break;
    }
    mid /= 2.f;
    return mid;
}

Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>::iterator begin, std::vector<Face*>::iterator end, AXIS_TYPE axis)
{
    size_t size = end - begin;
    size_t mid = size/2;

    if(size < 1)
    {
        return NULL;
    }
    if(size == 1)
    {
        return *begin;
    }

    BoundingBox* globalBbox = new BoundingBox(FLOAT_MAX, -FLOAT_MAX,
                                              FLOAT_MAX, -FLOAT_MAX,
                                              FLOAT_MAX, -FLOAT_MAX);
    BoundingBox* tmpBox;

    for(auto it = begin; it < end; it++)
    {
        tmpBox = (*it)->getBoundingBox();    
        
        if(globalBbox->xmax < tmpBox->xmax) { globalBbox->xmax = tmpBox->xmax;} 
        if(globalBbox->ymax < tmpBox->ymax) { globalBbox->ymax = tmpBox->ymax;} 
        if(globalBbox->zmax < tmpBox->zmax) { globalBbox->zmax = tmpBox->zmax;} 

        if(globalBbox->xmin > tmpBox->xmin) { globalBbox->xmin = tmpBox->xmin;} 
        if(globalBbox->ymin > tmpBox->ymin) { globalBbox->ymin = tmpBox->ymin;} 
        if(globalBbox->zmin > tmpBox->zmin) { globalBbox->zmin = tmpBox->zmin;} 
    }
    BVH* boundingVolume = new BVH();
    boundingVolume->bbox = globalBbox;

    //sort all objects and choose median
    switch(axis)
    {
        case X_AXIS:
            std::sort(begin, end, cmpBBoxx<Face>());         
            axis = Y_AXIS;
            break;
        case Y_AXIS:
            std::sort(begin, end, cmpBBoxy<Face>());         
            axis = Z_AXIS;
            break;
        case Z_AXIS:
            std::sort(begin, end, cmpBBoxz<Face>());         
            axis = X_AXIS;
            break;
    }


    boundingVolume->left = constructMeshBVH(VAO, begin, begin+mid, axis);
    boundingVolume->right = constructMeshBVH(VAO, begin+mid, end, axis);



    return boundingVolume;
}
Object* constructObjectBVH(const std::vector<vec3f>& VAO, std::vector<Object*>::iterator begin, std::vector<Object*>::iterator end, AXIS_TYPE axis)
{
    size_t size = end - begin;
    size_t mid = size/2;

    if(size < 1)
    {
        return NULL;
    }
    if(size == 1)
    {
        return *begin;
    }

    BoundingBox* globalBbox = new BoundingBox(FLOAT_MAX, -FLOAT_MAX,
                                              FLOAT_MAX, -FLOAT_MAX,
                                              FLOAT_MAX, -FLOAT_MAX);
    BoundingBox* tmpBox;

    for(auto it = begin; it < end; it++)
    {
        tmpBox = (*it)->getBoundingBox();    
        
        if(globalBbox->xmax < tmpBox->xmax) { globalBbox->xmax = tmpBox->xmax;} 
        if(globalBbox->ymax < tmpBox->ymax) { globalBbox->ymax = tmpBox->ymax;} 
        if(globalBbox->zmax < tmpBox->zmax) { globalBbox->zmax = tmpBox->zmax;} 

        if(globalBbox->xmin > tmpBox->xmin) { globalBbox->xmin = tmpBox->xmin;} 
        if(globalBbox->ymin > tmpBox->ymin) { globalBbox->ymin = tmpBox->ymin;} 
        if(globalBbox->zmin > tmpBox->zmin) { globalBbox->zmin = tmpBox->zmin;} 
    }
    BVH* boundingVolume = new BVH();
    boundingVolume->bbox = globalBbox;
    //sort all objects and choose median
    switch(axis)
    {
        case X_AXIS:
            std::sort(begin, end, cmpBBoxx<Object>());         
            axis = Y_AXIS;
            break;
        case Y_AXIS:
            std::sort(begin, end, cmpBBoxy<Object>());         
            axis = Z_AXIS;
            break;
        case Z_AXIS:
            std::sort(begin, end, cmpBBoxz<Object>());         
            axis = X_AXIS;
            break;
    }
    //std::cout << "BVH NODE with size: " << size << " BOUNDING BOX" << std::endl;
    //std::cout << "xmin, xmax: " << "(" << globalBbox->xmin << ", " << globalBbox->xmax << ")" << std::endl;
    //std::cout << "ymin, ymax: " << "(" << globalBbox->ymin << ", " << globalBbox->ymax << ")" << std::endl;
    //std::cout << "zmin, zmax: " << "(" << globalBbox->zmin << ", " << globalBbox->zmax << ")" << std::endl;

    boundingVolume->left = constructObjectBVH(VAO, begin, begin+mid, axis);
    boundingVolume->right = constructObjectBVH(VAO, begin+mid, end, axis);

    return boundingVolume;
}
