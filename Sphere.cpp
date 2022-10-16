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
    return true;
}
