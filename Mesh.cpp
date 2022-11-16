#include "Mesh.hpp"
#include <cmath>
#include <limits>
#include <iostream>
#include "mat4x4.hpp"


Mesh::Mesh()
{
    transformation = NULL;
    bbox = NULL;

    faces = {};    
    material_id = -1;

    isInstanced = false;
    baseMesh = NULL;
    resetTransform = false;
}
Mesh::Mesh(const Mesh& rhs)
{
    faces = rhs.faces;    
    material_id = rhs.material_id;

    isInstanced = rhs.isInstanced;
    if(rhs.transformation) {
        transformation = new mat4x4(*(rhs.transformation));
    }
    else {
        transformation = NULL;
    }

    if(rhs.bbox){
        bbox = new BoundingBox(*(rhs.bbox));
    }
    else {
        bbox = NULL;
    }

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

    if(transformation){
        delete transformation;
    }
    if(bbox) {
        delete bbox; 
    }
    //TODO deallocate BVH 
}

bool Mesh::intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intData)
{
    if(isInstanced)
    {
        Ray r = ray;
        if(transformation != NULL)
        {
            vec4f tmp;
            mat4x4 invM = inverse(*transformation);

            tmp = invM * vec4f(ray.o, 1.f); 
            r.o = vec3f(tmp.x, tmp.y, tmp.z);

            tmp = invM * vec4f(ray.d, 0.f); 
            r.d = vec3f(tmp.x, tmp.y, tmp.z);
        }

        bool hit = false;

        if(resetTransform)
        {
            hit = baseMesh->intersectRayResetTransform(VAO, r, intData);    
        }
        else
        {
            hit = baseMesh->intersectRay(VAO, r, intData);
        }
        if(hit)
        {
            intData.hitType = MESH;
            intData.material_id = this->material_id;


            if(transformation != NULL)
            {
                intData.intersectionPoint = ray.o + (ray.d * intData.t);

                vec4f tmp;
                //tmp = (*transformation) * vec4f(intData.intersectionPoint, 1.f);
                //intData.intersectionPoint = vec3f(tmp.x, tmp.y, tmp.z);


                mat4x4 invM = inverse(*transformation);
                tmp = transpose(invM) * vec4f(intData.normal, 0.f);
                intData.normal = norm(vec3f(tmp.x, tmp.y, tmp.z));
            }
            //notice that temp is carry data of a face
        }
        return hit;
    }


    Ray r = ray;
    if(transformation != NULL)
    {
        vec4f tmp;
        mat4x4 invM = inverse(*transformation);

        tmp = invM * vec4f(ray.o, 1.f); 
        r.o = vec3f(tmp.x, tmp.y, tmp.z);

        tmp = invM * vec4f(ray.d, 0.f); 
        r.d = vec3f(tmp.x, tmp.y, tmp.z);
    }

    bool hit = false;
    if(AccBVH != NULL)
    {
        hit = AccBVH->intersectRay(VAO, r, intData);
        if(hit)
        {
            intData.hitType = MESH;
            intData.material_id = this->material_id;
        }
        //return hit;
    }
    else
    {
        float min_t = std::numeric_limits<float>::infinity();
        hit = false;

        IntersectionData temp;
        //int faceIndex = 0;
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
            //faceIndex++;
        }
    }

    if(hit)
    {
        intData.hitType = MESH;
        intData.material_id = this->material_id;


        if(transformation != NULL)
        {
            intData.intersectionPoint = ray.o + (ray.d * intData.t);

            vec4f tmp;
            //tmp = (*transformation) * vec4f(intData.intersectionPoint, 1.f);
            //intData.intersectionPoint = vec3f(tmp.x, tmp.y, tmp.z);


            mat4x4 invM = inverse(*transformation);
            tmp = transpose(invM) * vec4f(intData.normal, 0.f);
            intData.normal = norm(vec3f(tmp.x, tmp.y, tmp.z));
        }
        //notice that temp is carry data of a face
    }
    return hit;
}

//vec3f Mesh::getSurfNormal(const std::vector<vec3f>& VAO, const IntersectionData& intersectionPoint) const 
//{
//
//    //return intersectionPoint.face.getSurfNormal(VAO, intersectionPoint);
//    //return this->faces[intersectionPoint.faceId -1].getSurfNormal(VAO, intersectionPoint);
//    vec3f a = VAO[intersectionPoint.v0_id];
//    vec3f b = VAO[intersectionPoint.v1_id];
//    vec3f c = VAO[intersectionPoint.v2_id];
//
//    return norm(cross(b-a, c-b));
//}

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

    if(isInstanced)
    {
        bbox = baseMesh->getBoundingBox(VAO);
        //if(resetTransform && baseMesh->transformation)
        //{
        //    *bbox = inverse(*(baseMesh->transformation)) * (*bbox);
        //}

        if(transformation)
        {
            *bbox = (*transformation) * (*bbox);
        }

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



    //std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    //std::cout << "MESH BOUNDING BOX with material: " << this->material_id << std::endl;
    //std::cout << "xmin, xmax: " << "(" << bbox->xmin << ", " << bbox->xmax << ")" << std::endl;
    //std::cout << "ymin, ymax: " << "(" << bbox->ymin << ", " << bbox->ymax << ")" << std::endl;
    //std::cout << "zmin, zmax: " << "(" << bbox->zmin << ", " << bbox->zmax << ")" << std::endl;
    //std::cout << " AFTER TRANSFORMATION " << std::endl;

    if(transformation)
    {
        *bbox = (*transformation) * (*bbox);
    }
    
    //std::cout << "MESH BOUNDING BOX with material: " << this->material_id << std::endl;
    //std::cout << "xmin, xmax: " << "(" << bbox->xmin << ", " << bbox->xmax << ")" << std::endl;
    //std::cout << "ymin, ymax: " << "(" << bbox->ymin << ", " << bbox->ymax << ")" << std::endl;
    //std::cout << "zmin, zmax: " << "(" << bbox->zmin << ", " << bbox->zmax << ")" << std::endl;
    //std::cout << "______________________________________________" << std::endl;

    return bbox;
}

BoundingBox* Mesh::getBoundingBox() const
{
    return bbox;
}

bool Mesh::intersectRayResetTransform(const std::vector<vec3f>& VAO, const Ray& r, IntersectionData& intData)
{
    //Ray r = ray;
    //if(transformation != NULL)
    //{
    //    vec4f tmp;
    //    mat4x4 invM = inverse(*transformation);

    //    tmp = invM * vec4f(ray.o, 1.f); 
    //    r.o = vec3f(tmp.x, tmp.y, tmp.z);

    //    tmp = invM * vec4f(ray.d, 0.f); 
    //    r.d = vec3f(tmp.x, tmp.y, tmp.z);
    //}
    if(isInstanced)
    {
        return baseMesh->intersectRay(VAO, r, intData);
    }

    bool hit = false;
    if(AccBVH != NULL)
    {
        hit = AccBVH->intersectRay(VAO, r, intData);
        if(hit)
        {
            intData.hitType = MESH;
            intData.material_id = this->material_id;
        }
        //return hit;
    }
    else
    {
        float min_t = std::numeric_limits<float>::infinity();
        hit = false;
        

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
    }

    if(hit)
    {
        intData.hitType = MESH;
        intData.material_id = this->material_id;


        //if(transformation != NULL)
        //{
        //    intData.intersectionPoint = ray.o + (ray.d * intData.t);

        //    vec4f tmp;
        //    //tmp = (*transformation) * vec4f(intData.intersectionPoint, 1.f);
        //    //intData.intersectionPoint = vec3f(tmp.x, tmp.y, tmp.z);


        //    mat4x4 invM = inverse(*transformation);
        //    tmp = transpose(invM) * vec4f(intData.normal, 0.f);
        //    intData.normal = norm(vec3f(tmp.x, tmp.y, tmp.z));
        //}
    }
    return hit;
}
