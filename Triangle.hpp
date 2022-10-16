#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "Face.hpp"
#include "Object.hpp"

class Triangle: public Object
{
    public:
    int material_id;
    Face indices;
};

#endif
