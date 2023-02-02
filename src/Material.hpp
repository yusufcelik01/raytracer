#ifndef __MATERIAL
#define __MATERIAL

#include "rtmath.hpp"
#include "Spectrum.hpp"

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
    union
    {
        bool isNormalized;
        bool isKdFresnel;
    };
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
    Spectrum ambient;
    Spectrum diffuse;
    Spectrum specular;
    //mirror
    Spectrum mirror;
    //confuctor fields
    float refraction_index;
    float absorption_index;
    //dielectric fields
    Spectrum absorption_coefficent;
    float phong_exponent = 1.f;
    float roughness = -0.f;
    //brdf
    BRDF brdf;
    bool isEmissive;//for object lights
    Spectrum radiance;//if emissive

    //constructors
    Material();
    Material(const Material& rhs);
    Spectrum computeBRDF(vec3f surfNorm, vec3f w_light, vec3f w_eye);

    private:
    Spectrum originalBlinnPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye);
    Spectrum originalPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye);
    Spectrum modifiedPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye);
    Spectrum modifiedBlinnPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye);
    Spectrum torranceSparrow(vec3f surfNorm, vec3f w_light, vec3f w_eye);
};


#endif
