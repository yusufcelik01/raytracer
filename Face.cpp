#include "Face.hpp"
#include <cmath>

Face::Face()
{
    v0_id = -1;
    v1_id = -1;
    v2_id = -1;
    material_id = -1;
}

Face::Face(const Face& rhs)
{
    v0_id = rhs.v0_id;
    v1_id = rhs.v1_id;
    v2_id = rhs.v2_id;
    material_id = rhs.material_id;
}

bool Face::intersectRay(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData)
{
        //vec3f a = VAO[v0_id -1];
        //vec3f b = VAO[v1_id -1];
        //vec3f c = VAO[v2_id -1];
        vec3f a = VAO[v0_id];
        vec3f b = VAO[v1_id];
        vec3f c = VAO[v2_id];
        
        float ax_bx = a.x - b.x;
        float ay_by = a.y - b.y;
        float az_bz = a.z - b.z;

        float ax_cx = a.x - c.x;
        float ay_cy = a.y - c.y;
        float az_cz = a.z - c.z;

        float ax_ox = a.x - r.o.x;
        float ay_oy = a.y - r.o.y;
        float az_oz = a.z - r.o.z;

        

        float detA = ax_bx * (ay_cy * r.d.z - az_cz * r.d.y) 
                   + ay_by * (r.d.x * az_cz - ax_cx * r.d.z) 
                   + az_bz * (ax_cx * r.d.y - ay_cy * r.d.x);
        if( detA == 0)
        {
            return false;
        }

        float beta = ( ax_ox * (ay_cy * r.d.z - r.d.y * az_cz)
                     + ay_oy * (r.d.x * az_cz - ax_cx * r.d.z)
                     + az_oz * (ax_cx * r.d.y - ay_cy * r.d.x)
                     ) / detA;

        float gamma = (  ax_bx * (ay_oy * r.d.z - az_oz * r.d.y)
                       + ay_by * (r.d.x * az_oz - ax_ox * r.d.z)
                       + az_bz * (ax_ox * r.d.y - ay_oy * r.d.x)
                      ) / detA;

        float t = ( ax_bx * (ay_cy * az_oz - ay_oy * az_cz)
                  + ay_by * (ax_ox * az_cz - ax_cx * az_oz)
                  + az_bz * (ax_cx * ay_oy - ay_cy * ax_ox)
                  )/ detA;
        if( beta + gamma <= 1.f
            && 0.f <= beta
            && 0.f <= gamma
            && t > 0.f) 
        {
            intData.t = t;
            intData.v0_id = v0_id;
            intData.v1_id = v1_id;
            intData.v2_id = v2_id;
            intData.material_id = this->material_id;
            intData.intersectionPoint = r.o + r.d * t;


            //vec3f a = VAO[v0_id -1];
            //vec3f b = VAO[v1_id -1];
            //vec3f c = VAO[v2_id -1];
            vec3f a = VAO[v0_id];
            vec3f b = VAO[v1_id];
            vec3f c = VAO[v2_id];

            intData.normal = norm(cross(b-a, c-b));
            return true;
        }

    
    return false;
}
vec3f Face::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const 
{
    vec3f a = VAO[v0_id];
    vec3f b = VAO[v1_id];
    vec3f c = VAO[v2_id];

    return norm(cross(b-a, c-b));
}

int Face::getMaterialId()
{
    return this->material_id;
}
