#include "Sphere.hpp"
#include <cmath>

bool Sphere::intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intData)
{
    vec3f c = VAO[center_vertex_id-1];
    

    vec3f o_min_c = ray.o - c;
    float d_dot__omc = dot(ray.d, o_min_c);
    float disc = (d_dot__omc*d_dot__omc) - (dot(ray.d, ray.d)*(dot(o_min_c, o_min_c) - radius*radius));

    if(disc < 0)
    {
        return false;
    }
  
    float t1 = (-d_dot__omc + sqrt(disc))/ dot(ray.d, ray.d);
    float t2 = (-d_dot__omc - sqrt(disc))/ dot(ray.d, ray.d);
       
    if(t1 < t2)
    {
        intData.t = t1;
    }
    else
    {
        intData.t = t2;
    }
    intData.hitType = SPHERE;
    //intData.v0_id = -1;
    //intData.v1_id = -1;
    //intData.v2_id = -1;
    intData.sphereCenterId = center_vertex_id;
    intData.material_id = this->material_id;
    intData.intersectionPoint = ray.o + (ray.d * intData.t);

    return true;
}


vec3f Sphere::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const
{
    return norm(intersectionPoint.intersectionPoint - VAO[this->center_vertex_id-1]);
    //return vec3f(0.f);
}

int Sphere::getMaterialId()
{
    return this->material_id;
}
