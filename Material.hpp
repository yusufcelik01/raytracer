#ifndef __MATERIAL
#define __MATERIAL

#include "rtmath.hpp"

typedef enum MaterialType
{
    REGULAR,
    MIRROR,
    CONDUCTOR,
    DIELECTRIC,
    AIR,
}MaterialType;

struct Material
{
    //bool is_mirror;
    MaterialType type; 
    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    //mirror
    vec3f mirror;
    //confuctor fields
    float refraction_index;
    float absorption_index;
    //dielectric fields
    vec3f absorption_coefficent;
    float phong_exponent = 1.f;
    float roughness = -0.f;
};


#endif
