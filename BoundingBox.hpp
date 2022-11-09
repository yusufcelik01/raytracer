#ifndef __BOUNDING__BOX__DEFINED
#define __BOUNDING__BOX__DEFINED

#include "rtmath.hpp"
#include "Ray.hpp"


class BoundingBox
{
    public:
        float xmin, xmax;
        float ymin, ymax;
        float zmin, zmax;

   BoundingBox();
   BoundingBox(float x_min, float x_max,
               float y_min, float y_max, 
               float z_min, float z_max);
   BoundingBox(const BoundingBox& rhs);
   //member functions
   bool hitRay(const Ray& r);
};

#endif
