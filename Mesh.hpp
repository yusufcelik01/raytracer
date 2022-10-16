#ifndef __MESH__
#define __MESH__

#include "Object.hpp"
#include "Face.hpp"
#include <vector>


class Mesh : Object
{
    public:
    int material_id;
    std::vector<Face> faces;
};


#endif
