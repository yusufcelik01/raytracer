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
            intData.disableShading = true;
        }
        else if(texture->decalMode == TEX_MODE_REPLACE_NORMAL)
        {
            //undo the normalization
            vec3f normalSample = norm(texture->sample(u,v)*2.f - vec3f(1.f));
            intData.normal = norm(intData.TBN * normalSample); 
        }
        else if(texture->decalMode == TEX_MODE_BUMP_NORMAL)
        {
            float epsilon = buffers.epsilon*0.5f;
            vec3f normal = intData.normal * texture->bumpFactor;
            //vec3f q = intData.intersectionPoint + normal * texture->sample(u,v).b;
            //vec3f qt = intData.dp_du + normal * texture->sample(u + 1.f/500.f, v).r;
            //vec3f qb = intData.dp_dv + normal * texture->sample(u,v + 1.f/500.f).r;
            //vec3f q_du = intData.dp_du + normal * texture->sample(u + epsilon, v).r;
            //vec3f q_dv = intData.dp_dv + normal * texture->sample(u,v + epsilon).r;

            float dh_du = texture->sample(u + epsilon, v).g - texture->sample(u, v).g; 
            float dh_dv = texture->sample(u, v + epsilon).g - texture->sample(u, v).g; 
            //float dh_du = texture->sampleDu(u, v).b - texture->sample(u, v).b; 
            //float dh_dv = texture->sampleDv(u, v).b - texture->sample(u, v).b; 
            vec3f dq_du = intData.dp_du + normal * dh_du;
            vec3f dq_dv = intData.dp_dv + normal * dh_dv;

            intData.normal = norm(cross(dq_dv, dq_du));
            //intData.normal = norm(cross(q_dv, q_du));
            //intData.normal = norm(cross(qt-qb, q-qt));
        }
    }
}
