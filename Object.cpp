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

