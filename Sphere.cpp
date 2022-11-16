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

bool Sphere::intersectRay(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData)
{
    Ray ray = r;
    if(transformation != NULL)
    {
        vec4f tmp;
        mat4x4 invM = inverse(*transformation);

        tmp = invM * vec4f(r.o, 1.f); 
        ray.o = vec3f(tmp.x, tmp.y, tmp.z);

        tmp = invM * vec4f(r.d, 0.f); 
        ray.d = vec3f(tmp.x, tmp.y, tmp.z);
    }

    //vec3f c = VAO[center_vertex_id-1];
    vec3f c = VAO[center_vertex_id];
    

    vec3f o_min_c = ray.o - c;
    float d_dot__omc = dot(ray.d, o_min_c);
    float disc = (d_dot__omc*d_dot__omc) - (dot(ray.d, ray.d)*(dot(o_min_c, o_min_c) - radius*radius));

    //std::cout << "Discriminant: " << disc << std::endl;
    if(disc <= 0)
    {
        return false;
    }
  
    float t1 = (-d_dot__omc + sqrt(disc))/ dot(ray.d, ray.d);
    float t2 = (-d_dot__omc - sqrt(disc))/ dot(ray.d, ray.d);
       
    if(t1 >= 0)
    {
        if(t2 >= 0)
        {
            intData.t = (t1 < t2) ? t1 : t2;
        }
        else
        {
            //return t1;
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
    //if(t1 < t2 && t1 > 0)
    //{
    //    intData.t = t1;
    //}
    //else if (t2 > 0)
    //{
    //    intData.t = t2;
    //}
    //else
    //{
    //    return false;
    //}
    intData.hitType = SPHERE;
    //intData.v0_id = -1;
    //intData.v1_id = -1;
    //intData.v2_id = -1;
    intData.sphereCenterId = center_vertex_id;
    intData.material_id = this->material_id;
    intData.intersectionPoint = ray.o + (ray.d * intData.t);
    intData.normal = norm(intData.intersectionPoint - c);

    if(transformation != NULL)
    {
        intData.intersectionPoint = r.o + (r.d * intData.t);
        //vec4f movedCenter = (*transformation) * vec4f(c, 1.f);
        //intData.normal = norm(intData.intersectionPoint - vec3f(movedCenter.x, movedCenter.y, movedCenter.z));

        vec4f tmp;
        //tmp = (*transformation) * vec4f(intData.intersectionPoint, 1.f);
        //intData.intersectionPoint = vec3f(tmp.x, tmp.y, tmp.z);


        mat4x4 invM = inverse(*transformation);
        tmp = transpose(invM) * vec4f(intData.normal, 0.f);
        intData.normal = norm(vec3f(tmp.x, tmp.y, tmp.z));
    }

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
