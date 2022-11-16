#include <limits>
#include <algorithm>
#include <iostream>

#include "BoundingBox.hpp"


BoundingBox::BoundingBox()
{
    xmin = xmax = ymin = ymax = zmin = zmax = 
        std::numeric_limits<float>::quiet_NaN();
}


BoundingBox::BoundingBox(float x_min, float x_max,
                         float y_min, float y_max, 
                         float z_min, float z_max)
{
    this->xmin = x_min; this->xmax = x_max;
    this->ymin = y_min; this->ymax = y_max;
    this->zmin = z_min; this->zmax = z_max;
}

BoundingBox::BoundingBox(const BoundingBox& rhs)
{
    this->xmin = rhs.xmin; this->xmax = rhs.xmax;
    this->ymin = rhs.ymin; this->ymax = rhs.ymax;
    this->zmin = rhs.zmin; this->zmax = rhs.zmax;
}

bool BoundingBox::hitRay(const Ray& r)
{
    float tx1, tx2;
    float ty1, ty2;
    float tz1, tz2;

    tx1 = (xmin - r.o.x)/r.d.x;
    tx2 = (xmax - r.o.x)/r.d.x;
    if(tx1 > tx2){std::swap(tx1, tx2);}
     
    ty1 = (ymin - r.o.y)/r.d.y;
    ty2 = (ymax - r.o.y)/r.d.y;
    if(ty1 > ty2){std::swap(ty1, ty2);}
     
    tz1 = (zmin - r.o.z)/r.d.z;
    tz2 = (zmax - r.o.z)/r.d.z;
    if(tz1 > tz2){std::swap(tz1, tz2);}

    float t1 = std::max(std::max(tx1, ty1), tz1);
    float t2 = std::min(std::min(tx2, ty2), tz2);

    bool ret = !(t1 > t2);
    //std::cout << "bbox hit " << ret << std::endl;
    return ret;
}

BoundingBox operator*(mat4x4 M, BoundingBox bbox)
{
    vec4f corners[8];
    corners[0] = vec4f(bbox.xmin, bbox.ymin, bbox.zmin, 1.f);
    corners[1] = vec4f(bbox.xmin, bbox.ymin, bbox.zmax, 1.f);
    corners[2] = vec4f(bbox.xmin, bbox.ymax, bbox.zmin, 1.f);
    corners[3] = vec4f(bbox.xmin, bbox.ymax, bbox.zmax, 1.f);
    corners[4] = vec4f(bbox.xmax, bbox.ymin, bbox.zmin, 1.f);
    corners[5] = vec4f(bbox.xmax, bbox.ymin, bbox.zmax, 1.f);
    corners[6] = vec4f(bbox.xmax, bbox.ymax, bbox.zmin, 1.f);
    corners[7] = vec4f(bbox.xmax, bbox.ymax, bbox.zmax, 1.f);

    for(int i=0; i < 8; i++)
    {
        corners[i] = M * corners[i];
    }

    BoundingBox retval(corners[0].x, corners[0].x,
                       corners[0].y, corners[0].y,
                       corners[0].z, corners[0].z);

    for(int i=1; i < 8; i++)
    {
        if(retval.xmin > corners[i].x) { retval.xmin = corners[i].x; }
        if(retval.ymin > corners[i].y) { retval.ymin = corners[i].y; }
        if(retval.zmin > corners[i].z) { retval.zmin = corners[i].z; }

        if(retval.xmax < corners[i].x) { retval.xmax = corners[i].x; }
        if(retval.ymax < corners[i].y) { retval.ymax = corners[i].y; }
        if(retval.zmax < corners[i].z) { retval.zmax = corners[i].z; }
    }

    return retval;
}
