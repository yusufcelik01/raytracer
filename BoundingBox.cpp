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
