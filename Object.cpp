#include "Object.hpp"

Object::Object()
{
    material_id = -1;
    bbox = NULL;
    transformation = NULL;
    motionBlur = NULL;
    textures = {};
}
Object::Object(const Object& rhs)
{
    material_id = rhs.material_id;
    material = rhs.material;
    if(rhs.transformation) 
    {
        transformation = new mat4x4(*(rhs.transformation));
    }
    else 
    {
        transformation = NULL;
    }

    if(rhs.bbox) 
    {
        bbox = new BoundingBox(*(rhs.bbox));
    }
    else
    {
        bbox = NULL;
    }
    if(rhs.motionBlur) 
    {
        motionBlur = new vec3f(*(rhs.motionBlur));
    }
    else
    {
        bbox = NULL;
    }

}
Object::~Object()
{
    if(transformation)
    {
        delete transformation;
    }
    if(bbox) 
    {
        delete bbox; 
    }
    if(motionBlur)
    {
        delete motionBlur;
    }
}

void Object::processTextures(IntersectionData& intData)
{
    float u = intData.texCoord.x;
    float v = intData.texCoord.y;
    for(Texture* texture: textures)
    {
        //std::cout << "mesh texture" << std::endl;
        if(texture->decalMode == TEX_MODE_REPLACE_KD)
        {
            intData.material.diffuse = texture->sample(u, v)/255.f;
        }
        else if(texture->decalMode == TEX_MODE_BLEND_KD)
        {
            intData.material.diffuse += texture->sample(u, v)/255.f;
            intData.material.diffuse /= 2.f;
        }
        else if(texture->decalMode == TEX_MODE_REPLACE_KS)
        {
            intData.material.specular = texture->sample(u, v)/255.f;
        }
        else if(texture->decalMode == TEX_MODE_REPLACE_ALL)
        {
            vec3f texSample = texture->sample(u, v)/255.f;
            intData.material.ambient  = texSample;
            intData.material.diffuse  = texSample; 
            intData.material.specular = texSample; 
        }
    }
}
