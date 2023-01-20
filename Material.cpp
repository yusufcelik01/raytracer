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
}
