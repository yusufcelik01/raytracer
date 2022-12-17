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
    textures = rhs.textures;
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

void Object::processTextures(const VertexBuffers& buffers, IntersectionData& intData)
{
    float u = intData.texCoord.x;
    float v = intData.texCoord.y;
    for(Texture* texture: textures)
    {
        if(texture->decalMode == TEX_MODE_REPLACE_KD)
        {
            intData.material.diffuse = texture->sample(u, v);
        }
        else if(texture->decalMode == TEX_MODE_BLEND_KD)
        {
            intData.material.diffuse += texture->sample(u, v);
            intData.material.diffuse /= 2.f;
        }
        else if(texture->decalMode == TEX_MODE_REPLACE_KS)
        {
            intData.material.specular = texture->sample(u, v);
        }
        else if(texture->decalMode == TEX_MODE_REPLACE_ALL)
        {
            vec3f texSample = texture->sample(u, v);
            intData.material.ambient  = texSample;
            intData.material.diffuse  = texSample; 
            intData.material.specular = texSample; 
        }
        else if(texture->decalMode == TEX_MODE_REPLACE_NORMAL)
        {
            //undo the normalization
            vec3f normalSample = norm(texture->sample(u,v)*2.f - vec3f(1.f));
            intData.normal = norm(intData.TBN * normalSample); 
        }
        else if(texture->decalMode == TEX_MODE_BUMP_NORMAL)
        {
            vec3f normal = intData.normal * buffers.epsilon * texture->bumpFactor;
            vec3f p = intData.intersectionPoint + normal * texture->sample(u,v);
            vec3f pt = intData.dp_du + normal * texture->sample(u + buffers.epsilon, v);
            vec3f pb = intData.dp_du + normal * texture->sample(u,v + buffers.epsilon);

            intData.normal = norm(cross(pt-pb, p-pt));
        }
    }
}
