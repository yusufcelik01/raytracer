#ifndef __MATERIAL
#define __MATERIAL

#include "rtmath.hpp"

typedef enum MaterialType
{
    MATERIAL_REGULAR,
    MATERIAL_MIRROR,
    MATERIAL_CONDUCTOR,
    MATERIAL_DIELECTRIC,
    MATERIAL_AIR
}MaterialType;


typedef enum BRDFType
{
    BRDF_ORIGINAL_BLINN_PHONG,
    BRDF_ORIGINAL_PHONG,
    BRDF_MODIFIED_BLINN_PHONG,
    BRDF_MODIFIED_PHONG,
    BRDF_TORRANCE_SPARROW
}BRDFType;

class BRDF
{
    public:
    float exp;
    bool isNormalized;
    BRDFType type;

    BRDFType getType();
    BRDF();
    BRDF(const BRDF& rhs);
};

class Material
{
    //bool is_mirror;
    public:
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
    //brdf
    BRDF brdf;

    //constructors
    Material();
    Material(const Material& rhs);
    vec3f computeBRDF(vec3f surfNorm, vec3f w_light, vec3f w_eye);

    private:
    vec3f originalBlinnPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye);
    vec3f originalPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye);
    //vec3f modifiedBlinnPhong(vec3f irradiance, vec3f surfNorm, vec3f w_light, vec3f w_eye);
    //vec3f modifiedPhong(vec3f irradiance, vec3f surfNorm, vec3f w_light, vec3f w_eye);
    //vec3f torranceSparrow(vec3f irradiance, vec3f surfNorm, vec3f w_light, vec3f w_eye);
};


#endif
