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

vec3f Material::computeBRDF(vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    vec3f reflectance(0.f);
    switch(brdf.getType())
    {
        case BRDF_ORIGINAL_BLINN_PHONG:
            reflectance = originalBlinnPhong(surfNorm, w_light, w_eye);
            break;
        case BRDF_ORIGINAL_PHONG:
            reflectance = originalPhong(surfNorm, w_light, w_eye);
            break;
        case BRDF_MODIFIED_BLINN_PHONG:
            reflectance = modifiedBlinnPhong(surfNorm, w_light, w_eye);
            break;
        case BRDF_MODIFIED_PHONG:
            reflectance = modifiedPhong(surfNorm, w_light, w_eye);
            break;
        case BRDF_TORRANCE_SPARROW:
            reflectance = torranceSparrow(surfNorm, w_light, w_eye);
            break;
        default:
            break;
    }
    return reflectance;
}

// various BRDFs 
// all the arguments are assumed to be normalized
vec3f Material::originalBlinnPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    float cosTheta = dot(surfNorm, w_light);
    if(cosTheta <= 0)
    {
        return vec3f(0.f);//theta > 90
    }

    //specular shading 
    vec3f half = norm(w_light + w_eye);
    float cosAlpha = dot(half, surfNorm);
    cosAlpha = std::max(pow(cosAlpha, phong_exponent), 0.0);

    return diffuse + (specular * cosAlpha / cosTheta); 
}

vec3f Material::originalPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    float cosTheta = dot(surfNorm, w_light);
    if(cosTheta <= 0)
    {
        return vec3f(0.f);//theta > 90
    }
    vec3f w_ri = reflect(surfNorm, w_light);
    float cosAlpha = dot(w_ri, w_eye);
    cosAlpha = pow(std::max(cosAlpha, 0.0f), phong_exponent); 
    
    return diffuse + (specular * cosAlpha / cosTheta);
}

vec3f Material::modifiedPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    float cosTheta = dot(surfNorm, w_light);
    if(cosTheta <= 0)
    {
        return vec3f(0.f);//theta > 90
    }

    vec3f w_ri = reflect(surfNorm, w_light);
    float cosAlpha = dot(w_ri, w_eye);
    cosAlpha = pow(std::max(cosAlpha, 0.0f), phong_exponent); 
    
    if(brdf.isNormalized == false)
    {
        return diffuse + (specular * cosAlpha);
    }
    else
    {
        return (diffuse + (specular * (phong_exponent + 2.f)/2.f * cosAlpha) ) / M_PI;
    }
    return diffuse + (specular * cosAlpha);
}

vec3f Material::modifiedBlinnPhong(vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    float cosTheta = dot(surfNorm, w_light);
    if(cosTheta <= 0)
    {
        return vec3f(0.f);//theta > 90
    }

    vec3f half = norm(w_light + w_eye);
    float cosAlpha = dot(half, surfNorm);
    cosAlpha = std::max(pow(cosAlpha, phong_exponent), 0.0);

    if(brdf.isNormalized)
    {
        return (diffuse + specular * (phong_exponent + 8.f) / 8.f * cosAlpha ) / M_PI;
    }
    else
    {
        return diffuse + (specular * cosAlpha); 
    }
}

vec3f Material::torranceSparrow(vec3f surfNorm, vec3f w_light, vec3f w_eye)
{
    float cosTheta = dot(surfNorm, w_light);
    if(cosTheta <= 0.f)
    {
        return vec3f(0.f);
    }
    vec3f w_h= norm(w_light + w_eye);
    float cosAlpha = dot(w_h, surfNorm);
    float D = (phong_exponent + 2.f)/(2* M_PI) 
             * pow(cosAlpha, phong_exponent);

    float NdotWi = cosTheta;
    float NdotWh = cosAlpha;
    float NdotWo = dot(surfNorm, w_eye);
    float WoDotWh = dot(w_eye, w_h);
    float GCommon = 2.f * NdotWh / WoDotWh;

    float G1 = GCommon * NdotWo;
    float G2 = GCommon * NdotWi;

    float G = std::min(1.f, std::min(G1, G2));

    //compute Fresnel reflection by Shclick's approx.
    //float tmp = (refraction_index - 1)/(refraction_index + 1);
    //float R0 = tmp * tmp;
    float R0 = pow(refraction_index-1, 2)/pow(refraction_index+1, 2);
    
    float cosBeta = dot(w_h, w_eye);
    float F = R0 + (1 - R0) * pow((1.0 - cosBeta), 5.0);

    vec3f reflectance(0.f);

    if(brdf.isKdFresnel)
    {
        reflectance += diffuse * ((1-F)/ M_PI);
    }
    else
    {
        reflectance += diffuse / M_PI;
    }

    reflectance += specular * ((D * F * G) / ( 4 * cosTheta * NdotWo));
    
    return reflectance;
}

//=========================BRDF=================
//BRDF member functions
BRDF::BRDF()
{
    type = BRDF_ORIGINAL_BLINN_PHONG;
    exp = 50.f;
    isNormalized = false;
}

BRDF::BRDF(const BRDF& rhs)
{
    type = rhs.type;
    exp = rhs.exp;
    isNormalized = rhs.isNormalized;
}

BRDFType BRDF::getType()
{
    return type;
}
