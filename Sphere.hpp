#ifndef __SPHERE__
#define __SPHERE__

#include "Object.hpp"

class Sphere: Object
{
    public:
    int material_id;
    int center_vertex_id;
    float radius;
};

#endif
