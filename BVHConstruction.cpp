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

//float getBBcenter(Object* obj)
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

//Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>& objects, AXIS_TYPE axis)
Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>::iterator begin, std::vector<Face*>::iterator end, AXIS_TYPE axis)
{
    //auto end = objects.end();
    //auto begin = objects.begin();

    //size_t size = objects.size();
    size_t size = end - begin;
    size_t mid = size/2;

    //std::cout << "BVH construction number of elements in this BVH: " << size << std::endl;
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

    //for(Face* face: objects)
    for(auto it = begin; it < end; it++)
    {
        tmpBox = (*it)->getBoundingBox();    
        //std::cout << "center of this BB: " << getBBcenter(**it, X_AXIS) << std::endl;
        
        if(globalBbox->xmax < tmpBox->xmax) { globalBbox->xmax = tmpBox->xmax;} 
        if(globalBbox->ymax < tmpBox->ymax) { globalBbox->ymax = tmpBox->ymax;} 
        if(globalBbox->zmax < tmpBox->zmax) { globalBbox->zmax = tmpBox->zmax;} 

        if(globalBbox->xmin > tmpBox->xmin) { globalBbox->xmin = tmpBox->xmin;} 
        if(globalBbox->ymin > tmpBox->ymin) { globalBbox->ymin = tmpBox->ymin;} 
        if(globalBbox->zmin > tmpBox->zmin) { globalBbox->zmin = tmpBox->zmin;} 

        //std::cout << "____________________________" << std::endl;
                  
    }
    BVH* boundingVolume = new BVH();
    boundingVolume->bbox = globalBbox;
    //std::cout << "BOUNDING BOX OF SIZE:  " << size << std::endl;
    //std::cout << "xmin: " << globalBbox->xmin << " xmax: " << globalBbox->xmax << std::endl
    //          << "ymin: " << globalBbox->ymin << " ymax: " << globalBbox->ymax << std::endl 
    //          << "zmin: " << globalBbox->zmin << " zmax: " << globalBbox->zmax << std::endl; 
    //std::cout << "_______________________________" << std::endl;



    //sort all objects and choose median
    switch(axis)
    {
        case X_AXIS:
            std::sort(begin, end, FaceCmpBBoxx());         
            axis = Y_AXIS;
            break;
        case Y_AXIS:
            std::sort(begin, end, FaceCmpBBoxy());         
            axis = Z_AXIS;
            break;
        case Z_AXIS:
            std::sort(begin, end, FaceCmpBBoxz());         
            axis = X_AXIS;
            break;
    }


    //for(auto it = begin; it < end; it++)
    //{
    //    tmpBox = (*it)->getBoundingBox();    
    //    std::cout << "center of this BB: " << getBBcenter(**it, X_AXIS) << std::endl;
    //}

    boundingVolume->left = constructMeshBVH(VAO, begin, begin+mid, axis);
    boundingVolume->right = constructMeshBVH(VAO, begin+mid, end, axis);
    //(*(begin + mid))->getBoundingBox();


    return boundingVolume;
}
//Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>& objects, AXIS_TYPE axis)
//{
//    return constructMeshBVH(VAO, objects, 0, objects.size() - 1, AXIS_TYPE);
//}
//Object* constructMeshBVH(const std::vector<vec3f>& VAO, std::vector<Face*>& objects, size_t first, size_t last, AXIS_TYPE axis)
//{
//    //auto end = objects.end();
//    //auto begin = objects.begin();
//
//    size_t size = last - first +1;
//    //size_t size = end - begin;
//    size_t mid = size/2;
//
//    std::cout << "BVH construction number of elements in this BVH: " << size << std::endl;
//    if(size < 1)
//    {
//        return NULL;
//    }
//    if(size == 1)
//    {
//        return *begin;
//    }
//
//    BoundingBox* globalBbox = new BoundingBox(FLOAT_MAX, -FLOAT_MAX,
//                                              FLOAT_MAX, -FLOAT_MAX,
//                                              FLOAT_MAX, -FLOAT_MAX);
//    BoundingBox* tmpBox;
//
//    //for(Face* face: objects)
//    for(auto it = begin; it < end; it++)
//    {
//        tmpBox = (*it)->getBoundingBox();    
//        std::cout << "center of this BB: " << getBBcenter(**it, X_AXIS) << std::endl;
//        
//        if(globalBbox->xmax < tmpBox->xmax) { globalBbox->xmax = tmpBox->xmax;} 
//        if(globalBbox->ymax < tmpBox->ymax) { globalBbox->ymax = tmpBox->ymax;} 
//        if(globalBbox->zmax < tmpBox->zmax) { globalBbox->zmax = tmpBox->zmax;} 
//
//        if(globalBbox->xmin > tmpBox->xmin) { globalBbox->xmin = tmpBox->xmin;} 
//        if(globalBbox->ymin > tmpBox->ymin) { globalBbox->ymin = tmpBox->ymin;} 
//        if(globalBbox->zmin > tmpBox->zmin) { globalBbox->zmin = tmpBox->zmin;} 
//        //std::cout << "xmin: " << globalBbox->xmin << " xmax: " << globalBbox->xmax << std::endl
//        //          << "ymin: " << globalBbox->ymin << " ymax: " << globalBbox->ymax << std::endl 
//        //          << "zmin: " << globalBbox->zmin << " zmax: " << globalBbox->zmax << std::endl; 
//
//        //std::cout << "____________________________" << std::endl;
//                  
//    }
//    BVH* boundingVolume = new BVH();
//    boundingVolume->bbox = globalBbox;
//
// 
//
//    //sort all objects and choose median
//    switch(axis)
//    {
//        case X_AXIS:
//            std::sort(begin, end, FaceCmpBBoxx);         
//            axis = Y_AXIS;
//            break;
//        case Y_AXIS:
//            std::sort(begin, end, FaceCmpBBoxy);         
//            axis = Z_AXIS;
//            break;
//        case Z_AXIS:
//            std::sort(begin, end, FaceCmpBBoxz);         
//            axis = X_AXIS;
//            break;
//    }
//
//
//    for(auto it = begin; it < end; it++)
//    {
//        tmpBox = (*it)->getBoundingBox();    
//        std::cout << "center of this BB: " << getBBcenter(**it, X_AXIS) << std::endl;
//    }
//
//    //boundingVolume->left = constructMeshBVH(VAO, begin, begin+mid, axis);
//    //boundingVolume->right = constructMeshBVH(VAO, begin+mid, end, axis);
//    //(*(begin + mid))->getBoundingBox();
//
//
//    return boundingVolume;
//}

