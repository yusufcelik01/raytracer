#include "parser.h"
#include "img.hpp"
#include "Ray.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <thread>
#include <mutex>
#include <random>
#include <cassert>

//#define NUMBER_OF_THREADS 8



vec3f deviateRay(vec3f originalRay, float roughness)
{
    std::random_device rn;
    std::mt19937 rand(rn());
    std::uniform_real_distribution<> randDistribution(-0.5 * roughness, 0.5 * roughness);
    float psi1 = randDistribution(rand);
    float psi2 = randDistribution(rand);

    ONB onb = ONB(originalRay);

    return norm(originalRay + (psi1 * onb.v) + (psi2 * onb.u) );
}

vec3f computeBlinnPhong(vec3f irradiance, vec3f surfNorm, vec3f w_light, vec3f w_eye, Material material)
{
    vec3f color = vec3f(0.f);

    
    float cosTheta = dot(surfNorm, w_light);
    cosTheta = std::max(cosTheta, 0.0f);
    color += material.diffuse * cosTheta * irradiance;

    //specular shading 
    vec3f half = norm(w_light + w_eye);
    float cosAlpha = dot(half, surfNorm);
    cosAlpha = std::max(pow(cosAlpha, material.phong_exponent), 0.0);

    //cosAlpha = material.phong_exponent
    color += material.specular * cosAlpha * irradiance;

    return color;
}

vec3f Scene::calculateLighting(Ray eyeRay, Material material, vec3f surfNorm, vec3f p)
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
        shadowRay.time = eyeRay.time;
        float distance = length(w_light); 
        if(rayQuery(shadowRay, dummy, true, distance))//if shadow
        {
            continue;
        }

        w_light = norm(w_light);
        float d_sqr = distance * distance;

        vec3f irradiance = light.intensity / d_sqr;
        color += computeBlinnPhong(irradiance, surfNorm, w_light, w_eye, material);
    }

    for(AreaLight light : area_lights)
    {
        //ONB onb = light.getONB();
        //float psi1, psi2;
        //std::random_device rand;
        //std::mt19937 rnGen(rand());
        //std::uniform_real_distribution<> randNum(-0.5*light.extent, 0.5*light.extent);

        //psi1 = randNum(rnGen);
        //psi2 = randNum(rnGen);

        vec3f lightSample;
        lightSample = light.getLightSample();
        //lightSample = light.position + onb.u * psi1 + onb.v * psi2;
        Ray shadowRay;
        shadowRay.o = p + surfNorm * shadow_ray_epsilon;
        shadowRay.d = norm(lightSample - shadowRay.o);
        shadowRay.time = eyeRay.time;
        vec3f w_light = lightSample - p;
        float distance = length(w_light);
        if(rayQuery(shadowRay, dummy, true, distance))//if shadow
        {
            continue;
        }
        w_light = norm(w_light);
        float d_sqr = distance * distance;

        float area = light.extent * light.extent;
        float cosTheta = dot(light.normal, -w_light);
        if( cosTheta < 0.f) {cosTheta = -cosTheta;}//bi-idirectional area light
        vec3f irradiance = light.radiance * (area * cosTheta / d_sqr);

        color += computeBlinnPhong(irradiance, surfNorm, w_light, w_eye, material);
    }

    return color;
}

bool Scene::rayQuery(Ray ray, IntersectionData& retData, bool isShadowRay, float maxT, float nearDist, vec3f gaze)
{
    IntersectionData closestObjData;
    closestObjData.material_id = -1;
    closestObjData.t = std::numeric_limits<float>::infinity(); 

    vec3f color = vec3f(0.f);

    bool hit = false;
    for(Object* object: objects)
    {
        IntersectionData intData;
        //if(object->intersectRay(VAO.vertexCoords, ray, intData))
        if(object->intersectRay(VAO, ray, intData))
        {
            if(intData.t < closestObjData.t && intData.t < maxT && intData.t > 0 && dot(ray.d * intData.t, gaze) > nearDist )
            {
                if(isShadowRay) {return true;}
                hit = true;
                closestObjData = IntersectionData(intData);
            }
        }


    }
    retData = closestObjData;
    return hit;

}
bool Scene::rayQuery(Ray ray, IntersectionData& retData, bool isShadowRay, float maxT)
{
    IntersectionData closestObjData;
    closestObjData.material_id = -1;
    closestObjData.t = std::numeric_limits<float>::infinity(); 

    vec3f color = vec3f(0.f);

    bool hit = false;
    for(Object* object: objects)
    {
        IntersectionData intData;
        //if(object->intersectRay(VAO.vertexCoords, ray, intData))
        if(object->intersectRay(VAO, ray, intData))
        {
            if(intData.t < closestObjData.t && intData.t < maxT && intData.t > 0)
            {
                if(isShadowRay) {return true;}
                hit = true;
                closestObjData = IntersectionData(intData);
            }
        }


    }
    retData = closestObjData;
    return hit;
}

vec3f Scene::getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium)
{
    return getRayColor(ray, depth, false, currentMedium, vec3f(0.f), 0.f);
}
vec3f Scene::getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium, vec3f gaze, float nearDist)
{
    if(depth < 0)
    {
        return vec3f(0.f, 0.f, 0.f);
    }

    IntersectionData closestObjData;
    bool hit = false;
    if(isPrimaryRay)
    {
        assert(length(gaze) > 0.01f );
        hit = rayQuery(ray, closestObjData, false, std::numeric_limits<float>::infinity(), nearDist, gaze);
    }
    else
    {
        hit = rayQuery(ray, closestObjData, false, std::numeric_limits<float>::infinity());
    }

    if(hit){
        Material objMaterial;
        //objMaterial = materials[closestObjData.material_id];
        objMaterial = closestObjData.material;
        vec3f n = closestObjData.normal;

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
            Ray reflectingRay;
            reflectingRay.d = reflect(n, -ray.d);
            reflectingRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon; 
            reflectingRay.time = ray.time;

            if(objMaterial.roughness != -0.f)
            {
                reflectingRay.d = deviateRay(reflectingRay.d, objMaterial.roughness); 
            }
            color += objMaterial.mirror * getRayColor(reflectingRay, depth-1, false, currentMedium);
        }
        else if(objMaterial.type == DIELECTRIC)
        {
            Ray reflectingRay, refractingRay;
            reflectingRay.d = reflect(n, -ray.d);
            reflectingRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon; 
            reflectingRay.time = ray.time;

            refractingRay.o = closestObjData.intersectionPoint - n * shadow_ray_epsilon;
            refractingRay.time = ray.time;

            float eta;
            if (currentMedium.type == AIR){ eta = 1.f / objMaterial.refraction_index; }
            else { eta = currentMedium.refraction_index ;}
            refractingRay.d = refract(n, ray.d, eta);


            if(refractingRay.d == vec3f(0.f))//total internal reflection
            {
                if(objMaterial.roughness != -0.f)
                {
                    reflectingRay.d = deviateRay(reflectingRay.d, objMaterial.roughness); 
                }
                color += getRayColor(reflectingRay, depth - 1, false, currentMedium);
            }
            else//refraction + reflection
            {
                if(objMaterial.roughness != -0.f)
                {
                    refractingRay.d = deviateRay(refractingRay.d, objMaterial.roughness); 
                    reflectingRay.d = deviateRay(reflectingRay.d, objMaterial.roughness); 
                }
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
            reflectingRay.time = ray.time;

            if(objMaterial.roughness != -0.f)
            {
                reflectingRay.d = deviateRay(reflectingRay.d, objMaterial.roughness); 
            }
            
            float reflectionRatio = conductorReflectionRatio(objMaterial.refraction_index, objMaterial.absorption_index, dot(-ray.d, n));
            color += reflectionRatio * objMaterial.mirror * getRayColor(reflectingRay, depth-1, false, currentMedium);
        }
        
        return color;
    }
    else {
        if(isPrimaryRay) {
            if(background_texture == NULL) {
                return this->background_color;
            }
            else {
                return background_texture->sample(ray.texCoord.s, ray.texCoord.t)*255.f;
            }
        }
        else {
            return vec3f(0, 0, 0);
        }
    }
}



//void Scene::render(Camera camera)
//{
//    vec3f e = camera.position;
//    vec3f u, v, w;    
//    v = norm(camera.up);
//    w = norm(-camera.gaze);
//    u = norm(cross(v, w));
//    v = norm(cross(w, u));
//
//    int nx = camera.image_width;
//    int ny = camera.image_height;
//    float l = camera.near_plane.x;
//    float r = camera.near_plane.y;
//    float b = camera.near_plane.z;
//    float t = camera.near_plane.w;
//
//    int x,y;
//    unsigned char* img = new unsigned char[camera.image_width* camera.image_height * 3];
//
//    vec3f m = e - w * camera.near_distance;
//    vec3f q = m + l * u + t * v;
//    Material airMedium;
//    airMedium.type = AIR;
//    airMedium.absorption_index = 0.f;
//    airMedium.refraction_index = 1.f;
//    airMedium.absorption_coefficent = vec3f(0.f);
//
//    //for(y = 511; y < camera.image_height; ++y)
//    for(y = 0; y < camera.image_height; ++y)
//    {
//        //for(x = 430; x < 450 /*camera.image_width*/; ++x)
//        for(x = 0; x < camera.image_width; ++x)
//        {
//            float s_u = (x + 0.5) * (r-l)/float(nx);
//            float s_v = (y + 0.5) * (t-b)/float(ny);
//
//            vec3f s = q + s_u * u - s_v * v;
//            Ray ray; 
//            ray.o = e;
//            ray.d = norm(s - e);
//
//            
//            vec3f color = getRayColor(ray, max_recursion_depth, true, airMedium);
//            vec3i c = clamp(vec3i(color), 0, 255);
//            
//            img[(camera.image_width*y + x)*3] = c.r;
//            img[(camera.image_width*y + x)*3 + 1] = c.g;
//            img[(camera.image_width*y + x)*3 + 2] = c.b;
//            //std::cout << "pixel DONE \n----------------"  << std::endl;
//        }
//        //break;
//        //std::cout << "row " << y << " completed" << std::endl; 
//    }
//    write_png(camera.image_name.c_str() , nx, ny, img);
//
//    delete[] img;
//}


class ImageRows
{
    private:
        int numOfRows = 0;
        int nextAvaliableRow = 0;;
        std::mutex lock;

    public:
        ImageRows() : numOfRows(0), nextAvaliableRow(0) {}
        ImageRows(int rowCount) : numOfRows(rowCount) {}

        int getNextAvaliableRow()
        {
            lock.lock();
            if(nextAvaliableRow < numOfRows)
            {
                int temp = nextAvaliableRow;
                nextAvaliableRow++;

                lock.unlock();
                return temp;
            }
            else 
            {
                lock.unlock();
                return -1;
            }
        }

};

struct RowRendererArg
{
    vec3f u, v, w;//camera orientation
    vec3f e;//camera position

    int nx, ny;// image resolution
    float l, r, b ,t;//image plane
    float pixelWidth, pixelHeight;
    vec3f q; //top-left corner of image plane
    Material initialMedium;
    int numberOfSamples;
    Camera* camera;
    UniformRandomGenerator pixelSampler, apertureSampler;

    ImageRows* rows;
    unsigned char* img;//output image data
};

void Scene::render(Camera camera)
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

    RowRendererArg threadArg;
    threadArg.u  = u;
    threadArg.v  = v;
    threadArg.w  = w;

    threadArg.e  = e;

    threadArg.nx = nx;
    threadArg.ny = ny;

    threadArg.l = l;
    threadArg.r = r;
    threadArg.b = b;
    threadArg.t = t;
    threadArg.q = q;
    threadArg.pixelWidth  = (r-l)/float(nx);
    threadArg.pixelHeight = (t-b)/float(ny);
    threadArg.numberOfSamples = camera.numSamples;

    threadArg.initialMedium = airMedium;

    threadArg.camera = &camera;
    threadArg.img = img;
    threadArg.rows = new ImageRows(ny);

    unsigned int NUMBER_OF_THREADS = std::thread::hardware_concurrency(); 
    std::cout << "Number of threads running: " << NUMBER_OF_THREADS << std::endl;
    std::thread threads[NUMBER_OF_THREADS];
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        //threads[i] = std::thread(&Scene::renderRow, this, &threadArg);
        threads[i] = std::thread(&Scene::renderRowMultiSampled, this, &threadArg);
    }


    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        threads[i].join();
    }

    //for(y = 511; y < camera.image_height; ++y)
    //for(y = 0; y < camera.image_height; ++y)
    //{
    //    //for(x = 430; x < 450 /*camera.image_width*/; ++x)
    //    for(x = 0; x < camera.image_width; ++x)
    //    {
    //        float s_u = (x + 0.5) * (r-l)/float(nx);
    //        float s_v = (y + 0.5) * (t-b)/float(ny);

    //        vec3f s = q + s_u * u - s_v * v;
    //        Ray ray; 
    //        ray.o = e;
    //        ray.d = norm(s - e);

    //        
    //        vec3f color = getRayColor(ray, max_recursion_depth, true, airMedium);
    //        vec3i c = clamp(vec3i(color), 0, 255);
    //        
    //        img[(camera.image_width*y + x)*3] = c.r;
    //        img[(camera.image_width*y + x)*3 + 1] = c.g;
    //        img[(camera.image_width*y + x)*3 + 2] = c.b;
    //        //std::cout << "pixel DONE \n----------------"  << std::endl;
    //    }
    //    //break;
    //    //std::cout << "row " << y << " completed" << std::endl; 
    //}
    write_png(camera.image_name.c_str() , nx, ny, img);

    delete[] img;
    delete threadArg.rows;
}



//render single sample for each pixel
void Scene::renderRow(void* void_arg)
{
    RowRendererArg* arg = (RowRendererArg*) void_arg;

    
    //TODO get new row to render
    int y;
    int x;
    y = arg->rows->getNextAvaliableRow();
    while(y != -1)
    {
        for(x = 0; x < arg->nx; ++x)
        {
            float s_u = (x + 0.5) * arg->pixelWidth;
            float s_v = (y + 0.5) * arg->pixelHeight;

            vec3f s = arg->q + s_u * arg->u - s_v * arg->v;//pixel sample
            Ray ray; 
            ray.o = arg->e;
            ray.d = norm(s - arg->e);


            vec3f color = getRayColor(ray, max_recursion_depth, true, arg->initialMedium);
            vec3i c = clamp(vec3i(color), 0, 255);

            arg->img[(arg->nx*y + x)*3] = c.r;
            arg->img[(arg->nx*y + x)*3 + 1] = c.g;
            arg->img[(arg->nx*y + x)*3 + 2] = c.b;
            //std::cout << "pixel DONE \n----------------"  << std::endl;
        }
        //TODO get new row to render
        y = arg->rows->getNextAvaliableRow();
    }
    

}



//void generateJitteredSamples(UniformRandomGenerator& rng, std::vector<vec2f>& samples, int numberOfSamples )
//{
//    //std::random_device rand;
//    //std::mt19937 rng1(rand());
//    //std::mt19937 rng2(rand());
//    //std::mt19937 rnGen;
//    //std::uniform_real_distribution<> randNum1(0, 1);
//    //std::uniform_real_distribution<> randNum2(0, 1);
//
//    int x, y; 
//    x = int(sqrt(numberOfSamples) + 0.5);
//    y = int(float(numberOfSamples) / x);
//    for(int i = 0 ; i < x; ++i)
//    {
//        for(int j = 0; j < y; ++j)
//        {
//            //float psi1 = randNum1(rng1);
//            //float psi2 = randNum2(rng2);
//            float psi1 = rng.getUniformRandNumber(0.f, 1.0f);
//            float psi2 = rng.getUniformRandNumber(0.f, 1.0f);
//
//            vec2f sample;
//            sample.x = float(i + psi1) / x;
//            sample.y = float(j + psi2) / y;
//
//            samples.push_back(sample);
//        }
//    }
//}

void generateJitteredSamples(UniformRandomGenerator& rng, std::vector<vec2f>& samples, int numberOfSamples )
{
    int x, y; 
    x = int(sqrt(numberOfSamples) + 0.5);
    y = int(float(numberOfSamples) / x);
    float width = 1.f/ x;
    float height = 1.f/ y;
    for(size_t i = 0; i < x; ++i)
    {
        for(size_t j = 0; j < y; ++j)
        {
            vec2f sample;
            sample.x = rng.getUniformRandNumber(i*width, (i+1)*width);
            sample.y = rng.getUniformRandNumber(j*height, (j+1)*height);

            samples.push_back(sample);
        }
    }
}


float gauss(float sigma, float x, float y)
{

    float tmp = (1.f/(2 * M_PI * sigma*sigma)) * pow(std::exp(1.0), -0.5 *( (x*x + y*y ) /(sigma*sigma)) );
    //std::cout << "gauss(" << x << ", " << y << ") = "  << tmp << std::endl;
    return tmp;
}

vec3f filterBox(const std::vector<vec3f>& sampleValues)
{
    vec3f acc(0.f);
    for(vec3f sample : sampleValues)
    {
        acc += sample;
    }
    return acc / sampleValues.size();
}

vec3f filterGauss(const std::vector<vec2f>& sampleCoords, const std::vector<vec3f>& sampleValues)
{
    float totalWeight = 0.f;
    vec3f totalValue = vec3f(0.f);

    size_t sampleSize = sampleValues.size();
    for(size_t i = 0; i < sampleSize; ++i)
    {
        float g = gauss(1/4.f, sampleCoords[i].x -0.5f, sampleCoords[i].y - 0.5f);
        totalWeight += g;
        totalValue += g * sampleValues[i];
    }
    return totalValue / totalWeight;
}

void Scene::renderRowMultiSampled(void* void_arg)
{
    RowRendererArg* arg = (RowRendererArg*) void_arg;

    std::random_device rn_seed;
    std::mt19937 rand_mt(rn_seed());
    std::uniform_real_distribution<> randTime(0, 1);


    int y;
    int x;
    y = arg->rows->getNextAvaliableRow();
    while(y != -1)//get another row if any left
    {
        for(x = 0; x < arg->nx; ++x)
        {
            std::vector<vec2f> samples  = {};
            std::vector<vec3f> sampleColors = {};
            float apertureSize = arg->camera->apertureSize;

            if(arg->numberOfSamples == 1)
            {
                samples.push_back(vec2f(0.5f, 0.5f));
            }
            else
            {
                UniformRandomGenerator pixelSampler;
                generateJitteredSamples(pixelSampler, samples, arg->numberOfSamples);
            }
            for(vec2f sample: samples)
            {
                vec2f backGroundCoords; 
                backGroundCoords.x = (x + sample.x)/ arg->nx;
                backGroundCoords.y = (y + sample.y)/ arg->ny;

                float s_u = (x + sample.x) * arg->pixelWidth;
                float s_v = (y + sample.y) * arg->pixelHeight;

                vec3f s = arg->q + s_u * arg->u - s_v * arg->v;//pixel sample
                Ray ray; 
                
                if(arg->camera->apertureSize < 0.f)
                {
                    ray.o = arg->e;
                    ray.d = norm(s - arg->e);
                }
                else//depth of field
                {
                    float psi1, psi2;
                    

                    psi1 = arg->apertureSampler.getUniformRandNumber(-0.5*apertureSize, 0.5*apertureSize);
                    psi2 = arg->apertureSampler.getUniformRandNumber(-0.5*apertureSize, 0.5*apertureSize);

                    vec3f u = arg->u;
                    vec3f v = arg->v;
                    vec3f o = arg->camera->position;

                    vec3f apertureSample = o + u * psi1 + v * psi2;

                    vec3f dir = norm(o - s);//TODO image plane is not behind lens
                    float t_fd = arg->camera->focusDistance / dot(dir, -arg->w);
                    vec3f p = o + t_fd * dir;

                    vec3f d = p - apertureSample;
                    ray.o = apertureSample;
                    ray.d = d;
                }


                ray.time =  randTime(rand_mt);
                ray.texCoord = backGroundCoords;
                //std::cout << "ray time: " << ray.time << std::endl;
                //vec3f sampleColor = getRayColor(ray, max_recursion_depth, true, arg->initialMedium);
                vec3f sampleColor = getRayColor(ray, max_recursion_depth, true, arg->initialMedium,  -arg->w, arg->camera->near_distance);

                sampleColors.push_back(sampleColor);
            }
            
            //apply box filter to samples
            vec3f pixelColor = vec3f(0.f);
            //pixelColor = filterBox(sampleColors); 
            pixelColor = filterGauss(samples, sampleColors); 
            //end box filter
            vec3i c = clamp(vec3i(pixelColor), 0, 255);

            arg->img[(arg->nx*y + x)*3] = c.r;
            arg->img[(arg->nx*y + x)*3 + 1] = c.g;
            arg->img[(arg->nx*y + x)*3 + 2] = c.b;
        }
        y = arg->rows->getNextAvaliableRow();
    }
    

}
