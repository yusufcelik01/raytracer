#include "Sphere.hpp"
#include <cmath>
#include <iostream>

Sphere::Sphere()
{
    center_vertex_id = -1;
    radius = 0;
    material_id = -1;
}
Sphere::Sphere(const Sphere& rhs)
{
    center_vertex_id = rhs.center_vertex_id;
    radius = rhs.radius;
    material_id = rhs.material_id;
    if(rhs.transformation)
    {
        transformation = new mat4x4(*(rhs.transformation));
    }
    else
    {
        transformation = NULL;
    }
}

Sphere::~Sphere()
{
    if(bbox)
    {
        delete bbox;
    }
}

bool Sphere::intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intData)
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


    vec3f c = VAO[center_vertex_id];
    vec3f o_min_c = r.o - c;
    float d_dot__omc = dot(r.d, o_min_c);
    float disc = (d_dot__omc*d_dot__omc) - (dot(r.d, r.d)*(dot(o_min_c, o_min_c) - radius*radius));
    if(disc <= 0)
    {
        return false;
    }
  
    float t1 = (-d_dot__omc + sqrt(disc))/ dot(r.d, r.d);
    float t2 = (-d_dot__omc - sqrt(disc))/ dot(r.d, r.d);
       
    if(t1 >= 0)
    {
        if(t2 >= 0)
        {
            intData.t = (t1 < t2) ? t1 : t2;
        }
        else
        {
            intData.t = t1;
        }
    }
    else
    {
        if(t2 >= 0)
        {
            intData.t = t2;
        }
        else
        {
            return false;
        }
    }
    intData.hitType = SPHERE;
    intData.sphereCenterId = center_vertex_id;
    intData.material_id = this->material_id;

    //object space intersection point and normal
    intData.intersectionPoint = r.o + (r.d * intData.t);
    intData.normal = norm(intData.intersectionPoint - c);

    //transfrom them back to world space
    tmp = compositeTransformation * vec4f(intData.intersectionPoint, 1.f);
    intData.intersectionPoint = vec3f(tmp.x, tmp.y, tmp.z);
    
    tmp = transpose(invM) * vec4f(intData.normal, 0.f);
    intData.normal = norm(vec3f(tmp.x, tmp.y, tmp.z));

    return true;

}


vec3f Sphere::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const
{
    //return norm(intersectionPoint.intersectionPoint - VAO[this->center_vertex_id-1]);
    return norm(intersectionPoint.intersectionPoint - VAO[this->center_vertex_id]);
    //return vec3f(0.f);
}

int Sphere::getMaterialId()
{
    return this->material_id;
}

BoundingBox* Sphere::getBoundingBox(const std::vector<vec3f>& VAO) 
{
    if(bbox != NULL)
    {
        return bbox;
    }

    vec3f c = VAO[center_vertex_id];
    bbox = new BoundingBox(c.x - radius, c.x + radius,
                           c.y - radius, c.y + radius,
                           c.z - radius, c.z + radius);

    if(transformation)
    {
        *bbox = (*transformation) * (*bbox);
    }
    return bbox;
}

BoundingBox* Sphere::getBoundingBox() const
{
    return bbox;
}
