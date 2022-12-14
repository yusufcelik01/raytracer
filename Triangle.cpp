#include "Triangle.hpp"
#include <cstddef>

Triangle::Triangle()
{
    indices = Face();
    material_id = -1;
}

Triangle::Triangle(const Triangle& rhs) : Object(rhs)
{
    indices = rhs.indices;
    //material_id = rhs.material_id;
    //material = rhs.material;
    //if(rhs.transformation)
    //{
    //    transformation = new mat4x4(*(rhs.transformation));
    //}
    //else
    //{
    //    transformation = NULL;
    //}
    //if(rhs.bbox) {
    //    bbox = new BoundingBox(*(rhs.bbox));
    //}
    //if(rhs.motionBlur) {
    //    motionBlur = new vec3f(*(rhs.motionBlur));
    //}
}

Triangle::~Triangle()
{
    //if(bbox)
    //{
    //    delete bbox;        
    //}
    indices.~Face();
}

bool Triangle::intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intData) 
{
    mat4x4 compositeTransformation(1.f), invM(1.f);
    Ray r = ray;

    if(transformation != NULL)
    {
        compositeTransformation = *transformation;
    }
    if(motionBlur)
    {
        compositeTransformation = translate(*motionBlur * r.time) * compositeTransformation;
    }
    invM = inverse(compositeTransformation);
    
    vec4f tmp;
    tmp = invM * vec4f(ray.o, 1.f); 
    r.o = vec3f(tmp.x, tmp.y, tmp.z);

    tmp = invM * vec4f(ray.d, 0.f); 
    r.d = vec3f(tmp.x, tmp.y, tmp.z);
    r.time = ray.time;

    bool hit = false;

    hit = this->indices.intersectRay(VAO, r, intData);
    if(hit)
    {
        //intData.obj = this;
        intData.hitType = TRIANGLE;
        intData.material_id = this->material_id;
        intData.material = this->material;

        if(transformation != NULL)
        {
            //intData.intersectionPoint = ray.o + (ray.d * intData.t);
            tmp = compositeTransformation * vec4f(intData.intersectionPoint, 1.f);
            intData.intersectionPoint = vec3f(tmp.x, tmp.y, tmp.z);

            tmp = transpose(invM) * vec4f(intData.normal, 0.f);
            intData.normal = norm(vec3f(tmp.x, tmp.y, tmp.z));
        }
    }

    return hit;
}

//vec3f Triangle::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const
//{
//    //TODO dummy 
//    //Triangle* triangle = intersectionPoint.obj;
//    vec3f a = VAO[this->indices.v0_id - 1];
//    vec3f b = VAO[this->indices.v1_id - 1];
//    vec3f c = VAO[this->indices.v2_id - 1];
//
//    return norm(cross(b-a, c-b));
//}

int Triangle::getMaterialId()
{
    return this->material_id;
}

BoundingBox* Triangle::getBoundingBox(const std::vector<vec3f>& VAO)
{
    if(bbox != NULL)
    {
        return bbox;
    }
    bbox = indices.getBoundingBox(VAO);     

    if(transformation)
    {
        *bbox = (*transformation) * (*bbox);
    }

    return bbox;
}

BoundingBox* Triangle::getBoundingBox() const
{
    return bbox;
}



