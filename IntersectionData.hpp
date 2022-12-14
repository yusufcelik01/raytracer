#ifndef __INTERSECTION__POINT__DATA
#define __INTERSECTION__POINT__DATA

#include "Material.hpp"

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
    union
    {
        struct{ int v0_id, v1_id, v2_id;};//triangle indices
        struct{ int sphereCenterId; };
    };

    
    vec3f intersectionPoint;
    int material_id;
    Material material;
    vec3f normal;
    vec2f texCoord; 
};

#endif
