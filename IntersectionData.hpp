#ifndef __INTERSECTION__POINT__DATA
#define __INTERSECTION__POINT__DATA

#include "Material.hpp"
#include "Matrix.hpp"

enum hitObjectType
{
    NONE,
    SPHERE,
    TRIANGLE,
    MESH
};

class IntersectionData //this is where i break oop and go back to C
{
    public:
    enum hitObjectType hitType;
    float t;

    vec3f intersectionPoint;
    int material_id;
    Material material;
    vec3f normal;
    vec2f texCoord; 
    Matrix TBN;
    vec3f dp_du, dp_dv;// surface points along u and v 
};

#endif
