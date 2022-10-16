#ifndef __MATERIAL
#define __MATERIAL

#include "rtmath.hpp"

typedef enum MaterialType
{
    REGULAR,
    MIRROR,
    CONDUCTOR,
    DIELECTRIC
}MaterialType;

struct Material
{
    //bool is_mirror;
    MaterialType type; 
    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    vec3f mirror;
    float phong_exponent;
};


#endif
