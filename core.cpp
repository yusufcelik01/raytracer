#include "parser.h"
#include "img.hpp"
#include "Ray.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <limits>

vec3f parser::Scene::getObjNorm(const IntersectionData& data)
{
    vec3f n;
    vec3f a,b,c;
    switch(data.hitType)
    {
        case SPHERE:
            n = norm(data.intersectionPoint - vertex_data[data.sphereCenterId - 1]);
            break;
        case TRIANGLE://fall through mesh
        case MESH:
            a = vertex_data[data.v0_id - 1]; 
            b = vertex_data[data.v1_id - 1]; 
            c = vertex_data[data.v2_id - 1]; 
            
            n = norm(cross(b-a, c-b));

            break;
        //default:
        //    n = vec3f(0.f);
    }

    return n;
}

vec3f parser::Scene::calculateLighting(Ray eyeRay, Material material, vec3f surfNorm, vec3f p)
{
    //for now only calculate diffuse & spcular shading
    //for only point lights
    IntersectionData dummy;
    vec3f w_eye = -norm(eyeRay.d);
    vec3f color = vec3f(0.f);
    for(PointLight light : point_lights)
    {
        vec3f w_light = (light.position - p);
        Ray shadowRay;
        shadowRay.o = p + surfNorm * shadow_ray_epsilon;
        shadowRay.d = norm(light.position - shadowRay.o);
        float distance = length(w_light); 
        if(rayQuery(shadowRay, dummy, true, distance))//if shadow
        {
            continue;
        }

        w_light = norm(w_light);
        float d_sqr = distance * distance;

        vec3f irradiance = light.intensity / d_sqr;
        //diffuse shading
        float cosTheta = dot(surfNorm, w_light);
        cosTheta = std::max(cosTheta, 0.0f);
        color += material.diffuse * cosTheta * irradiance;

        //specular shading 
        vec3f half = norm(w_light + w_eye);
        float cosAlpha = dot(half, surfNorm);
        cosAlpha = std::max(pow(cosAlpha, material.phong_exponent), 0.0);

        //cosAlpha = material.phong_exponent
        color += material.specular * cosAlpha * irradiance;
    }

    return color;
}

bool parser::Scene::rayQuery(Ray ray, IntersectionData& retData, bool isShadowRay, float maxT)
{
    IntersectionData closestObjData;
    closestObjData.material_id = -1;
    closestObjData.t = std::numeric_limits<float>::infinity(); 

    vec3f color = vec3f(0.f);

    bool hit = false;
    for(Sphere sphere : spheres)
    {
        IntersectionData intData;
        if(sphere.intersectRay(vertex_data, ray, intData))
        {
            if(intData.t < closestObjData.t && intData.t < maxT && intData.t > 0)
            {
                if(isShadowRay) {return true;}
                hit = true;
                closestObjData = IntersectionData(intData);
            }
        }

    }

    for(Triangle triangle : triangles)
    {
        IntersectionData intData;
        if(triangle.intersectRay(vertex_data, ray, intData))
        {
            if(intData.t < closestObjData.t && intData.t < maxT && intData.t > 0)
            {
                if(isShadowRay) {return true;}
                hit = true;
                closestObjData = intData;
            }
        }

    }

    int meshID = 1;
    for(Mesh mesh: meshes)
    {
        IntersectionData intData;
        if(mesh.intersectRay(vertex_data, ray, intData))
        {
            if(intData.t < closestObjData.t && intData.t < maxT && intData.t > 0)
            {

                if(isShadowRay ) {
                //std::cout << "============" << std::endl;
                //std::cout << "hit meshID: " << meshID << std::endl;
                //std::cout << "t: " << intData.t << std::endl;
                //std::cout << "============" << std::endl;

                    return true;}
                hit = true;
                closestObjData = intData;
            }
        }
        meshID++;

    }
    retData = closestObjData;
    //if(closestObjData.hitType == MESH){
    //    std::cout << "hit mesh" << std::endl;
    //}
    //else if (closestObjData.hitType == SPHERE){
    //    std::cout << "hit sphere" << std::endl;
    //}

    return hit;
}

vec3f parser::Scene::getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium)
{
    if(depth < 0)
    {
        return vec3f(0.f, 0.f, 0.f);
    }

    IntersectionData closestObjData;
    bool hit = false;
    hit = rayQuery(ray, closestObjData, false, std::numeric_limits<float>::infinity());

    if(hit){
        Material objMaterial = materials[closestObjData.material_id - 1];
        vec3f n = getObjNorm(closestObjData);

        vec3f color = vec3f(0.f);
        if(currentMedium.type == AIR)
        //if not we are inside an dielectric
        //object and light can not directly contribute
        {
            color += calculateLighting(ray, objMaterial, n, closestObjData.intersectionPoint);        
            color += objMaterial.ambient * ambient_light;
        }
        if(dot(n, ray.d) > 0)//if we are inside an object
        {
            n = -n;
        }

        //get refraction_index lightging
        if(objMaterial.type == MIRROR)
        {
            //std::cout <<"MIRROR ENTER" << std::endl;
            Ray reflectingRay;
            reflectingRay.d = reflect(n, -ray.d);
            reflectingRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon; 
            color += objMaterial.mirror * getRayColor(reflectingRay, depth-1, false, currentMedium);
        }
        else if(objMaterial.type == DIELECTRIC)
        {
            Ray reflectingRay, refractingRay;
            reflectingRay.d = reflect(n, -ray.d);
            reflectingRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon; 
            refractingRay.o = closestObjData.intersectionPoint - n * shadow_ray_epsilon;

            float eta;
            if (currentMedium.type == AIR){ eta = 1.f / objMaterial.refraction_index; }
            else { eta = currentMedium.refraction_index ;}
            refractingRay.d = refract(n, ray.d, eta);

            if(refractingRay.d == vec3f(0.f))//total internal reflection
            {
                //std::cout << "Total internal reflection" << std::endl;
                color += getRayColor(reflectingRay, depth - 1, false, currentMedium);
            }
            else//refraction + reflection
            {
                float reflectionRatio;
                if(currentMedium.type == AIR)
                {
                    reflectionRatio = dielectricReflectionRatio(currentMedium.refraction_index, objMaterial.refraction_index, dot(-ray.d, n), dot(n, -refractingRay.d) );
                }
                else
                {
                    reflectionRatio = dielectricReflectionRatio(currentMedium.refraction_index, 1.0f, dot(-ray.d, n), dot(n, -refractingRay.d) );
                }

                //TODO enable reflections on dielectrics
                color += reflectionRatio * getRayColor(reflectingRay, depth - 1, false, currentMedium);
                if(currentMedium.type == AIR)
                {
                    //if we are inside air now we will be entering a dielectric
                    color += (1.0 - reflectionRatio) * getRayColor(refractingRay, depth - 1, false, objMaterial);
                }
                else
                {
                    //if we are inside of a dielectric refracting ray will be exiting this medium
                    Material air;
                    air.type = AIR;
                    air.refraction_index = 1.f;
                    air.absorption_index = 0.f;
                    air.absorption_coefficent = vec3f(0.f);
                    color += (1.0 - reflectionRatio) * getRayColor(refractingRay, depth - 1, false, air);
                }
            }

            //color += objMaterial.refraction_index * getRayColor(reflectingRay, depth-1, false, currentRefractionIndex);

            //TODO
            //add current medium's refraction index to getRayColor function
            //then call the next function with new refraction index
            //depending on whether we are exiting or not
            //refract(n, ray, objMaterial.refraction_index/)
            if(currentMedium.type != AIR)
            {
                float attunuationDistance = length(ray.o - closestObjData.intersectionPoint);
                color = color * (exp(attunuationDistance * (-objMaterial.absorption_coefficent)));
            }

        }
        else if(objMaterial.type == CONDUCTOR)
        {
            Ray reflectingRay;
            reflectingRay.d = reflect(n, -ray.d);
            reflectingRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon; 

            float reflectionRatio = conductorReflectionRatio(objMaterial.refraction_index, objMaterial.absorption_index, dot(-ray.d, n));
            color += reflectionRatio * objMaterial.mirror * getRayColor(reflectingRay, depth-1, false, currentMedium);
        }
        
        return color;
    }
    else {
        if(isPrimaryRay) {
            return this->background_color;
        }
        else {
            return vec3f(0, 0, 0);
        }
    }
}


void parser::Scene::render(Camera camera)
{
    vec3f e = camera.position;
    vec3f u, v, w;    
    v = norm(camera.up);
    w = norm(-camera.gaze);
    u = norm(cross(v, w));
    v = norm(cross(w, u));

    int nx = camera.image_width;
    int ny = camera.image_height;
    float l = camera.near_plane.x;
    float r = camera.near_plane.y;
    float b = camera.near_plane.z;
    float t = camera.near_plane.w;

    int x,y;
    unsigned char* img = new unsigned char[camera.image_width* camera.image_height * 3];

    vec3f m = e - w * camera.near_distance;
    vec3f q = m + l * u + t * v;
    Material airMedium;
    airMedium.type = AIR;
    airMedium.absorption_index = 0.f;
    airMedium.refraction_index = 1.f;
    airMedium.absorption_coefficent = vec3f(0.f);

    //for(y = 511; y < camera.image_height; ++y)
    for(y = 0; y < camera.image_height; ++y)
    {
        //for(x = 430; x < 450 /*camera.image_width*/; ++x)
        for(x = 0; x < camera.image_width; ++x)
        {
            float s_u = (x + 0.5) * (r-l)/float(nx);
            float s_v = (y + 0.5) * (t-b)/float(ny);

            vec3f s = q + s_u * u - s_v * v;
            Ray ray; 
            ray.o = e;
            ray.d = norm(s - e);

            
            vec3f color = getRayColor(ray, max_recursion_depth, true, airMedium);
            vec3i c = clamp(vec3i(color), 0, 255);
            
            img[(camera.image_width*y + x)*3] = c.r;
            img[(camera.image_width*y + x)*3 + 1] = c.g;
            img[(camera.image_width*y + x)*3 + 2] = c.b;
            //std::cout << "pixel DONE \n----------------"  << std::endl;
        }
        //break;
        //std::cout << "row " << y << " completed" << std::endl; 
    }
    write_png(camera.image_name.c_str() , nx, ny, img);

    delete[] img;
}



void parser::Scene::render(size_t cameraId)
{
}
