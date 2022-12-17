#include "Sphere.hpp"
#include <cmath>
#include <iostream>

Sphere::Sphere()
{
    center_vertex_id = -1;
    radius = 0;
    material_id = -1;
}
Sphere::Sphere(const Sphere& rhs) : Object(rhs)
{
    center_vertex_id = rhs.center_vertex_id;
    radius = rhs.radius;
    //material_id = rhs.material_id;
    //material = rhs.material;
    //if(rhs.transformation) {
    //    transformation = new mat4x4(*(rhs.transformation));
    //}
    //else {
    //    transformation = NULL;
    //}

    //if(rhs.bbox) {
    //    bbox = new BoundingBox(*(rhs.bbox));
    //}
    //if(rhs.motionBlur) {
    //    motionBlur = new vec3f(*(rhs.motionBlur));
    //}
}

Sphere::~Sphere()
{
}

//bool Sphere::intersectRay(const std::vector<vec3f>& VAO, const Ray& ray, IntersectionData& intData)
bool Sphere::intersectRay(const VertexBuffers& vertexBuffers, const Ray& ray, IntersectionData& intData)
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


    vec3f c = vertexBuffers.vertexCoords[center_vertex_id];
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
    intData.material_id = this->material_id;
    intData.material = this->material;

    //object space intersection point and normal
    intData.intersectionPoint = r.o + (r.d * intData.t);
    intData.normal = norm(intData.intersectionPoint - c);

    //calculate the texture coordinates 
    //before transforming them back to world coords
    vec3f surfacePoint = norm(intData.intersectionPoint - c);
    //float theta = acos(surfacePoint.y/radius);
    float theta = acos(surfacePoint.y/1.f);
    float phi = atan2(surfacePoint.z, surfacePoint.x);

    vec2f texCoord;
    texCoord.s = (-phi + M_PI)/(2*M_PI);
    texCoord.t = theta/M_PI;
    intData.texCoord = texCoord;
    //std::cout << "sphere tex coords: " << texCoord.x << ", "<< texCoord.y <<std::endl;
    vec3f T = norm(vec3f(2*M_PI*surfacePoint.z,
                         0.f,
                         -2*M_PI*surfacePoint.x));
    vec3f B = norm(vec3f(M_PI*surfacePoint.y*cos(phi), 
                         -radius * M_PI * sin(theta),
                         M_PI * surfacePoint.y * sin(phi)));
    intData.TBN = Matrix(3,3);
    for(size_t m = 0; m < 3; m++)
    {
        intData.TBN.data[m][0] = T[m];
        intData.TBN.data[m][1] = B[m];
        intData.TBN.data[m][2] = intData.normal[m];
    }
                                    
    
    //TODO assign intData.TNB
    this->processTextures(vertexBuffers, intData);

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
    if(motionBlur)
    {
        BoundingBox initBox = *bbox;
        BoundingBox finalBox = translate(*motionBlur) * (*bbox);
        *bbox = getCompositeBox(initBox, finalBox);
    }
    return bbox;
}

BoundingBox* Sphere::getBoundingBox() const
{
    return bbox;
}
