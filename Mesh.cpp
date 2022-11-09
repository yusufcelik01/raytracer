#include "Mesh.hpp"
#include <cmath>
#include <limits>


Mesh::Mesh()
{
    faces = {};    
    material_id = -1;
}
Mesh::Mesh(const Mesh& rhs)
{
    faces = rhs.faces;    
    material_id = rhs.material_id;
}

Mesh::~Mesh()
{
    if(bbox != NULL)
    {
        delete bbox;
    }
    for(Face* face: faces)
    {
        if(face)
        {
            delete face;
        }
    }
    //TODO deallocate BVH 
}

bool Mesh::intersectRay(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData)
{
    if(AccBVH != NULL)
    {
        bool hit = AccBVH->intersectRay(VAO, r, intData);
        if(hit)
        {
            intData.hitType = MESH;
            intData.material_id = this->material_id;
        }
        return hit;
    }
    float min_t = std::numeric_limits<float>::infinity();
    bool hit = false;

    IntersectionData temp;
    int faceIndex = 0;
    for(Face* face: faces)
    {
        if(face->intersectRay(VAO, r, temp))
        {
            hit = true;
            if(temp.t < min_t)
            {
                min_t = temp.t;
                intData = temp;
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
    vec3f a = VAO[intersectionPoint.v0_id];
    vec3f b = VAO[intersectionPoint.v1_id];
    vec3f c = VAO[intersectionPoint.v2_id];

    return norm(cross(b-a, c-b));
}

int Mesh::getMaterialId()
{
    return this->material_id;
}

BoundingBox* Mesh::getBoundingBox(const std::vector<vec3f>& VAO)
{
    if(bbox != NULL)
    {
        return bbox;
    }
    bbox = new BoundingBox();
    bbox->xmax = -std::numeric_limits<float>::max();
    bbox->ymax = -std::numeric_limits<float>::max();
    bbox->zmax = -std::numeric_limits<float>::max();

    bbox->xmin = std::numeric_limits<float>::max();
    bbox->ymin = std::numeric_limits<float>::max();
    bbox->zmin = std::numeric_limits<float>::max();

    BoundingBox* faceBox;
    for(Face* face : faces)
    {
        faceBox = face->getBoundingBox(VAO);
        if(bbox->xmax < faceBox->xmax) { bbox->xmax = faceBox->xmax;} 
        if(bbox->ymax < faceBox->ymax) { bbox->ymax = faceBox->ymax;} 
        if(bbox->zmax < faceBox->zmax) { bbox->zmax = faceBox->zmax;} 

        if(bbox->xmin > faceBox->xmin) { bbox->xmin = faceBox->xmin;} 
        if(bbox->ymin > faceBox->ymin) { bbox->ymin = faceBox->ymin;} 
        if(bbox->zmin > faceBox->zmin) { bbox->zmin = faceBox->zmin;} 
    }

    return bbox;
}

BoundingBox* Mesh::getBoundingBox() const
{
    return bbox;
}
