#include "Material.hpp"

Material::Material()
{
    type = MATERIAL_REGULAR;
    ambient = 0.f;
    diffuse = 0.f;
    specular = 0.f;
    mirror = 0.f;
    phong_exponent = 1.f;
    roughness = -0.f;
    //confuctor fields
    refraction_index = 1.f;
    absorption_index = 1.f;
    //dielectric fields
    absorption_coefficent = 1.f;
    brdf = BRDF_ORIGINAL_BLINN_PHONG;
}

Material::Material(const Material& rhs)
{
    type = rhs.type;
    ambient = rhs.ambient;
    diffuse = rhs.diffuse;

    specular = rhs.specular;
    mirror = rhs.mirror;
    refraction_index = rhs.refraction_index;
    absorption_index = rhs.absorption_index;
    absorption_coefficent = rhs.absorption_coefficent;
    phong_exponent = rhs.phong_exponent; 
    roughness = rhs.roughness;
    brdf = rhs.brdf;
}

vec3f Material::computeBRDF(vec3f irradiance, vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    vec3f color(0.f);
    switch(brdf)
    {
        case BRDF_ORIGINAL_BLINN_PHONG:
            color = originalBlinnPhong(irradiance, surfNorm, w_light, w_eye);
            break;
        case BRDF_ORIGINAL_PHONG:
            break;
        case BRDF_MODIFIED_BLINN_PHONG:
            break;
        case BRDF_MODIFIED_PHONG:
            break;
        case BRDF_TORRANCE_SPARROW:
            break;
        default:
            break;
    }
    return color;
}


vec3f Material::originalBlinnPhong(vec3f irradiance, vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    vec3f color = vec3f(0.f);
    
    float cosTheta = dot(surfNorm, w_light);
    cosTheta = std::max(cosTheta, 0.0f);
    color += diffuse * cosTheta * irradiance;

    //specular shading 
    vec3f half = norm(w_light + w_eye);
    float cosAlpha = dot(half, surfNorm);
    cosAlpha = std::max(pow(cosAlpha, phong_exponent), 0.0);

    //cosAlpha = phong_exponent
    color += specular * cosAlpha * irradiance;

    return color;
}
