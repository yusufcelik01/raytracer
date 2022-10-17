#include "Mesh.hpp"
#include <cmath>
#include <limits>

bool Mesh::intersectRay(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData)
{
    float min_t = std::numeric_limits<float>::infinity();
    bool hit = false;

    IntersectionData temp;
    int faceIndex = 0;
    for(Face face: faces)
    {
        if(face.intersectRay(VAO, r, temp))
        {
            hit = true;
            if(temp.t < min_t)
            {
                min_t = temp.t;
                intData = temp;
                //intData.faceId = faceIndex;
                intData.v0_id = face.v0_id;
                intData.v1_id = face.v1_id;
                intData.v2_id = face.v2_id;
            }
        }
        faceIndex++;
    }

    if(hit)
    {
        intData.hitType = MESH;
        intData.material_id = this->material_id;
        //notice that temp is carry data of a face
    }
    return hit;
}

vec3f Mesh::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const 
{

    //return intersectionPoint.face.getSurfNormal(VAO, intersectionPoint);
    //return this->faces[intersectionPoint.faceId -1].getSurfNormal(VAO, intersectionPoint);
    vec3f a = VAO[intersectionPoint.v0_id -1];
    vec3f b = VAO[intersectionPoint.v0_id -1];
    vec3f c = VAO[intersectionPoint.v0_id -1];

    return norm(cross(b-a, c-b));
}

int Mesh::getMaterialId()
{
    return this->material_id;
}
