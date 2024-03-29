#include "parser.h"
#include "img.hpp"
#include "Ray.hpp"
#include "tonemap.hpp"
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


vec3f Scene::calculateLighting(const Camera& camera, Ray eyeRay, Material material, vec3f surfNorm, vec3f p)
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
        float cosTheta = dot(w_light, surfNorm);
        float d_sqr = distance * distance;

        vec3f irradiance = light.intensity / d_sqr;
        color += irradiance * material.computeBRDF(surfNorm, w_light, w_eye) * cosTheta;
    }

    for(AreaLight light : area_lights)
    {
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
        float cosAlpha = dot(light.normal, -w_light);
        if( cosAlpha < 0.f) {cosAlpha = -cosAlpha;}//bi-idirectional area light
        vec3f irradiance = light.radiance * (area * cosAlpha / d_sqr);

        float cosTheta = dot(w_light, surfNorm);
        color += irradiance * material.computeBRDF(surfNorm, w_light, w_eye) * cosTheta;
    }

    for(DirectionalLight light : directional_lights)
    {
        vec3f w_light = norm(-light.direction);
        Ray shadowRay; 
        shadowRay.o = p + surfNorm * shadow_ray_epsilon;
        shadowRay.d = w_light;
        shadowRay.time = eyeRay.time;
        if(rayQuery(shadowRay, dummy, true, FLOAT_MAX))
        {
            continue;
        }

        float cosTheta = dot(w_light, surfNorm);
        color += light.radiance * material.computeBRDF(surfNorm, w_light, w_eye) * cosTheta;
    }

    for(SpotLight light : spot_lights)
    {
        vec3f w_light = (light.position -p); 
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

        float cosAlpha = dot(light.direction, -w_light); 
        float alpha = acos(cosAlpha);
        if (alpha > light.covarageAngle)
        {
            continue; 
        }
        else if( alpha > light.fallOffAngle)
        {
            float cosFallOf = cos(light.fallOffAngle);
            float cosCoverage = cos(light.covarageAngle);
            float s = (cosAlpha - cosCoverage) 
                      / (cosFallOf - cosCoverage);
            s = s*s;
            s = s*s;//s^4
            irradiance = irradiance * s;
            //irradiance.r = irradiance.g = 0;
            //irradiance /= 4.f;
        }
        //else in no-fall of area

        float cosTheta = dot(w_light, surfNorm);
        color += irradiance * material.computeBRDF(surfNorm, w_light, w_eye) * cosTheta;
    }

    //TODO env light
    if(env_light != NULL)
    {
        UniformRandomGenerator rng;    
        vec3f dir;
        while(true)//rejection sample hemisphere
        {
            dir = vec3f(rng.getUniformRandNumber(-1.f, 1.f),
                        rng.getUniformRandNumber(-1.f, 1.f),
                        rng.getUniformRandNumber(-1.f, 1.f));
            if(length(dir) <= 1.f)
            {
                dir = norm(dir); 
                break;
            }
        }

        dir.y = (dir.y < 0.f) ? -dir.y : dir.y;//select the upper hemisphere

        ONB tbn = ONB(vec3f(0.f, 1.f, 0.f));
        //TODO this is not sampling the upper hemisphere this is constant
        //fix this
        Matrix onbTransform(3,3);
        
        for(int i = 0; i < 3; ++i)
        {
            //TBN->data[i][0] = tangentMat.data[0][i];//T
            //TBN->data[i][1] = tangentMat.data[1][i];//B
            onbTransform.data[i][0] = tbn.v[i];
            onbTransform.data[i][1] = tbn.n[i];
            onbTransform.data[i][2] = tbn.u[i]; 
        }
        vec3f lightDir = onbTransform * dir;

        vec3f radiance = env_light->sample(lightDir) * 2.f * M_PI;
        float cosTheta = dot(lightDir, surfNorm);
        color += radiance * material.computeBRDF(surfNorm, lightDir, w_eye) * cosTheta;
    }
    
    if(camera.rendererParams.pathTracing == false || camera.rendererParams.nextEvent == true)
    {
        for(SphereLight* light : sphere_lights)
        {
            SampledPoint lightPoint = light->sampleIlluminationPoint(VAO, p); 
            vec3f lightSample = lightPoint.point;

            if(lightSample.x != lightSample.x) 
            {
                //std::cerr << "sphere light sampling error" << std::endl;
                continue; 
            }
            Ray shadowRay;
            shadowRay.o = p + surfNorm * shadow_ray_epsilon;
            shadowRay.d = norm(lightSample - shadowRay.o);
            shadowRay.time = eyeRay.time;
            vec3f w_light = lightSample - p;
            float distance = length(w_light);
            float d_sqr = distance * distance;
            if(rayQuery(shadowRay, dummy, true, distance*0.97))//if shadow
            {
                continue;
            }
            w_light = norm(w_light);
            if(w_light.x != w_light.x) 
            {
                //std::cerr << "sphere light light direction NAN error" << std::endl;
                continue; 
            }

            vec3f radiance = light->radiance/(lightPoint.prob) ;

            float cosTheta = dot(w_light, surfNorm);
            color += radiance * material.computeBRDF(surfNorm, w_light, w_eye) * cosTheta;
        }

        for(MeshLight* light : mesh_lights)
        {
            //SampledPoint lightPoint = light->sampleIlluminationPoint(VAO, p); 
            vec3f Lpoint(0.f);
            vec3f Lnormal(0.f);
            float Larea = 0;;
            float Lprob = 0;

            int retStatus = light->sampleIlluminationPoint(VAO, p, Lpoint, Lnormal, Larea, Lprob); 
            //std::cout << "sample the point " << std::endl;
            //vec3f lightSample = lightPoint.point;
            vec3f lightSample = Lpoint;

            if(retStatus == -1) 
            {
                //std::cerr << "mesh light sampling error" << std::endl;
                continue; 
            }
            Ray shadowRay;
            shadowRay.o = p + surfNorm * shadow_ray_epsilon;
            shadowRay.d = norm(lightSample - shadowRay.o);
            shadowRay.time = eyeRay.time;
            vec3f w_light = lightSample - p;
            float distance = length(w_light);
            float d_sqr = distance * distance;
            if(rayQuery(shadowRay, dummy, true, distance*0.98))//if shadow
            {
                continue;
            }
            w_light = norm(w_light);
            float cosAlpha = dot(-w_light, Lnormal);
            if(cosAlpha < 0)
            {
                continue;
            }

            if(w_light.x != w_light.x) 
            {
                //std::cerr << "mesh light light direction NAN error" << std::endl;
                continue; 
            }

            vec3f irradiance = light->radiance * (Larea * cosAlpha)/(Lprob* d_sqr) ;

            float cosTheta = dot(w_light, surfNorm);
            color += irradiance * material.computeBRDF(surfNorm, w_light, w_eye) * cosTheta;

        }
    }


    if(material.isEmissive)
    {
        //TODO
        //put the cosine theta term here
        color += material.radiance;
    }


    return color;
}

bool Scene::rayQuery(Ray ray, IntersectionData& retData, bool isShadowRay, float maxT, float nearDist, vec3f gaze)
{
    IntersectionData closestObjData;
    closestObjData.material_id = -1;
    closestObjData.t = std::numeric_limits<float>::infinity(); 

    //vec3f color = vec3f(0.f);

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

    //vec3f color = vec3f(0.f);

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

//vec3f Scene::getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium)
//{
//    return getRayColor(ray, depth, false, currentMedium, vec3f(0.f), 0.f);
//}
//vec3f Scene::getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium, vec3f gaze, float nearDist)
vec3f Scene::getRayColor(Ray ray, int depth, bool isPrimaryRay, Material currentMedium, const Camera& camera)
{
    float nearDist = camera.near_distance;
    vec3f gaze = camera.gaze;
    //if(depth < 0)
    //{
    //    if(camera.rendererParams.russianRoulette == true)
    //    {
    //        UniformRandomGenerator rng;
    //        if(rng.getUniformRandNumber(0.f, 1.f) > ray.throughput)
    //        {
    //            return vec3f(0.f);
    //        }
    //    }
    //    else
    //    {
    //        return vec3f(0.f, 0.f, 0.f);
    //    }
    //}
    if(camera.rendererParams.russianRoulette == true)
    {
        UniformRandomGenerator rng;
        if(rng.getUniformRandNumber(0.f, 1.f) > ray.throughput)
        {
            return vec3f(0.f);
        }
    }
    else if(depth < 0)
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
        if(ray.type == RAY_TYPE_GLOBAL_ILLUMINATION_RAY
                && objMaterial.isEmissive == true
                && camera.rendererParams.nextEvent == true)
        {//hit an object light. kill this ray 
         //since we are doing next event est.
            return vec3f(0.f);
        }

        vec3f color = vec3f(0.f);
        if(currentMedium.type == MATERIAL_AIR)
        //if not we are inside an dielectric
        //object and light can not directly contribute
        {
            if(closestObjData.disableShading){
                color = closestObjData.material.diffuse;
            }
            else {
                color += calculateLighting(camera, ray, objMaterial, n, closestObjData.intersectionPoint);        
                if(camera.rendererParams.pathTracing)
                {
                    //sample hemisphere and trace it
                    ONB onb(n);//surface normal
                    mat4x4 transformONB = getONBTransformation(onb);
                    
                    UniformRandomGenerator rng;
                    float xi1 = rng.getUniformRandNumber(0.f, 1.f);
                    float xi2 = rng.getUniformRandNumber(0.f, 1.f);
                    vec3f sampledDir(std::nan("1"));
                    float sampleProb = 0.f;
                    if(camera.rendererParams.importanceSampling == false)
                    {
                        sampledDir = uniformSampleHemiSphere(xi1, xi2);
                        sampleProb = 1.f/(2* M_PI);
                    }
                    else
                    {
                        //TODO importance sample here
                        sampledDir = cosineSampleHemiSphere(xi1, xi2);
                        //notice that z may be 0 
                        //in this case we would get a zero probabalility
                        //hence division by zero
                        //we will solve this problem a few lines later
                        //by cancelling the cosTheta in rendering equation 
                        //by this cosTheta(sampleProb has cosTheta) 
                        //the actual reason we get 0
                        sampleProb = cosineHemispherePdf(sampledDir.z);
                    }
                    //transform the sampled hemisphere vector to world space
                    vec4f rayDir = transformONB * vec4f(sampledDir, 0.f);
                    Ray globalRay;
                    globalRay.d = norm(vec3f(rayDir.x, rayDir.y, rayDir.z));
                    globalRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon;
                    globalRay.time = ray.time; 
                    
                    globalRay.type = RAY_TYPE_GLOBAL_ILLUMINATION_RAY;
                    vec3f throughtputSpectrum = objMaterial.computeBRDF(n, globalRay.d, -norm(ray.d));
                    globalRay.throughput = ray.throughput;
                    globalRay.throughput *= getMaxComp(throughtputSpectrum);
                    //TODO later set the ray throughput here
                    //apply the rendering equation
                    float cosTheta = dot(n, globalRay.d);
                    vec3f irradiance = getRayColor(globalRay, depth -1, false, currentMedium, camera);
                    //divide by its probability also
                    if(camera.rendererParams.importanceSampling)
                    {
                        irradiance = irradiance * M_PI;
                        color += irradiance * throughtputSpectrum;
                        //if we are using importance sampling
                        //the cosineTheta in the rendering equation 
                        //and the probability of the sampled ray
                        //cancels each other out
                    }
                    else
                    {
                        //divide by uniform sampling probability 1/(2 PI)
                        irradiance = irradiance * (2 * M_PI);
                        color += irradiance * throughtputSpectrum * cosTheta;
                    }

                }
                else
                {
                    color += objMaterial.ambient * ambient_light;
                }
            }
        }
        if(dot(n, ray.d) > 0)//if we are inside an object
        {
            n = -n;
        }

        //get refraction_index lightging
        if(objMaterial.type == MATERIAL_MIRROR)
        {
            Ray reflectingRay;
            reflectingRay.d = reflect(n, -ray.d);
            reflectingRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon; 
            reflectingRay.time = ray.time;
            reflectingRay.type = RAY_TYPE_RECURSING_RAY;
            reflectingRay.throughput = ray.throughput * getMaxComp(objMaterial.mirror);

            if(objMaterial.roughness != -0.f)
            {
                reflectingRay.d = deviateRay(reflectingRay.d, objMaterial.roughness); 
            }
            //color += objMaterial.mirror * getRayColor(reflectingRay, depth-1, false, currentMedium);
            color += objMaterial.mirror * getRayColor(reflectingRay, depth-1, false, currentMedium, camera);
        }
        else if(objMaterial.type == MATERIAL_DIELECTRIC)
        {
            Ray reflectingRay, refractingRay;
            reflectingRay.d = reflect(n, -ray.d);
            reflectingRay.o = closestObjData.intersectionPoint + n * shadow_ray_epsilon; 
            reflectingRay.time = ray.time;

            refractingRay.o = closestObjData.intersectionPoint - n * shadow_ray_epsilon;
            refractingRay.time = ray.time;
            reflectingRay.type = RAY_TYPE_RECURSING_RAY;
            reflectingRay.throughput = ray.throughput;

            float eta;
            if (currentMedium.type == MATERIAL_AIR){ eta = 1.f / objMaterial.refraction_index; }
            else { eta = currentMedium.refraction_index ;}
            refractingRay.d = refract(n, ray.d, eta);


            if(refractingRay.d == vec3f(0.f))//total internal reflection
            {
                if(objMaterial.roughness != -0.f)
                {
                    reflectingRay.d = deviateRay(reflectingRay.d, objMaterial.roughness); 
                }
                color += getRayColor(reflectingRay, depth - 1, false, currentMedium, camera);
            }
            else//refraction + reflection
            {
                if(objMaterial.roughness != -0.f)
                {
                    refractingRay.d = deviateRay(refractingRay.d, objMaterial.roughness); 
                    reflectingRay.d = deviateRay(reflectingRay.d, objMaterial.roughness); 
                }
                float reflectionRatio;
                if(currentMedium.type == MATERIAL_AIR)
                {
                    reflectionRatio = dielectricReflectionRatio(currentMedium.refraction_index, objMaterial.refraction_index, dot(-ray.d, n), dot(n, -refractingRay.d) );
                }
                else
                {
                    reflectionRatio = dielectricReflectionRatio(currentMedium.refraction_index, 1.0f, dot(-ray.d, n), dot(n, -refractingRay.d) );
                }

                //TODO enable reflections on dielectrics
                color += reflectionRatio * getRayColor(reflectingRay, depth - 1, false, currentMedium, camera);
                if(currentMedium.type == MATERIAL_AIR)
                {
                    //if we are inside air now we will be entering a dielectric
                    color += (1.0 - reflectionRatio) * getRayColor(refractingRay, depth - 1, false, objMaterial, camera);
                }
                else
                {
                    //if we are inside of a dielectric refracting ray will be exiting this medium
                    Material air;
                    air.type = MATERIAL_AIR;
                    air.refraction_index = 1.f;
                    air.absorption_index = 0.f;
                    air.absorption_coefficent = vec3f(0.f);
                    color += (1.0 - reflectionRatio) * getRayColor(refractingRay, depth - 1, false, air, camera);
                }
            }

            //color += objMaterial.refraction_index * getRayColor(reflectingRay, depth-1, false, currentRefractionIndex, camera);

            //TODO
            //add current medium's refraction index to getRayColor function
            //then call the next function with new refraction index
            //depending on whether we are exiting or not
            //refract(n, ray, objMaterial.refraction_index/)
            if(currentMedium.type != MATERIAL_AIR)
            {
                float attunuationDistance = length(ray.o - closestObjData.intersectionPoint);
                color = color * (exp(attunuationDistance * (-objMaterial.absorption_coefficent)));
            }

        }
        else if(objMaterial.type == MATERIAL_CONDUCTOR)
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
            color += reflectionRatio * objMaterial.mirror * getRayColor(reflectingRay, depth-1, false, currentMedium, camera);
        }
        
        return color;
    }
    else {
        if(env_light != NULL)
        {
            return env_light->sample(norm(ray.d));
        }
        else if(isPrimaryRay) {
            if(background_texture != NULL) {
                return background_texture->sample(ray.texCoord.s, ray.texCoord.t)*255.f;
            }
            else {
                return this->background_color;
            }
            //if(background_texture == NULL) {
            //    return this->background_color;
            //}
            //else {
            //    return background_texture->sample(ray.texCoord.s, ray.texCoord.t)*255.f;
            //}
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


class ProgressBar
{
    //TODO make thread safe
    private: 
        int current, total;
        int lastNumSize;
        
    public:
        ProgressBar() : current(0), total(100), lastNumSize(2) {
            printf("progress: 0%%");
            fflush(stdout);

        }
        ProgressBar(int t) : current(0), total(t), lastNumSize(2) {

            printf("progress bar init %d\nprogress: 0%%", total);
            fflush(stdout);
        }
        //ProgressBar(int c, int t) : current(c), total(t), lastNumSize(2) {
        //    printf("progress: 0%");
        //    fflush(stdout);
        //}

        void incrProgress()
        {
            //TODO display progress bar
            current++;
            while(lastNumSize--)
            {
                printf("\b \b");
            }
            fflush(stdout);
            lastNumSize = printf("%f%%", getPercent());
            fflush(stdout);
        }
        float getPercent(){ return float(current)/total*100.f; }
        int getTotal() { return total; }
};

class ImageRows
{
    private:
        int numOfRows = 0;
        int nextAvaliableRow = 0;;
        ProgressBar progress;
        std::mutex lock;

    public:
        ImageRows() : numOfRows(0), nextAvaliableRow(0) {}
        ImageRows(int rowCount) : numOfRows(rowCount), nextAvaliableRow(0), progress(rowCount) {}

        int getNextAvaliableRow()
        {
            lock.lock();
            if(nextAvaliableRow < numOfRows)
            {
                int temp = nextAvaliableRow;
                nextAvaliableRow++;
                progress.incrProgress();
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
    //unsigned char* img;//output image data
    float* img;
};

void Scene::render(Camera camera)
{
    unsigned int NUMBER_OF_THREADS = std::thread::hardware_concurrency(); 
    //NUMBER_OF_THREADS = 1; 
    std::cout << "Number of threads running: " << NUMBER_OF_THREADS << std::endl;
    std::thread threads[NUMBER_OF_THREADS];

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

    //int x,y;
    float* img = new float[camera.image_width* camera.image_height * 3];

    vec3f m = e - w * camera.near_distance;
    vec3f q = m + l * u + t * v;
    Material airMedium;
    airMedium.type = MATERIAL_AIR;
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

    for(unsigned int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        //threads[i] = std::thread(&Scene::renderRow, this, &threadArg);
        threads[i] = std::thread(&Scene::renderRowMultiSampled, this, &threadArg);
    }


    for(unsigned int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        threads[i].join();
    }

    if(camera.image_name.substr(camera.image_name.size() -3) == std::string("exr"))//hdr tone map check
    {
        const char* err = NULL;
        SaveEXR(img, camera.image_width, camera.image_height, 3, 0, camera.image_name.c_str(), &err);
        std::cout << "writing to file: " << camera.image_name << std::endl;

        std::string ldrOutput = camera.image_name.substr(0, camera.image_name.size() - 3) + "png";
        int pixelCount = camera.image_width* camera.image_height * 3;
        unsigned char* ldrImg = NULL;
        ldrImg = tonemap::photographicToneMap(img, nx, ny, 3, 
                        camera.TMOArgs.keyValue, 
                        camera.TMOArgs.burnPercent, 
                        camera.TMOArgs.saturation, 
                        camera.TMOArgs.gamma);

        if(ldrImg)
        {
            write_png(ldrOutput.c_str() , nx, ny, ldrImg);
            std::cout << "writing to file: " << ldrOutput << std::endl;
        }
    }
    else//no tonemapping and hdr
    {
        int pixelCount = camera.image_width* camera.image_height * 3;
        unsigned char* ldrImg = new unsigned char[pixelCount]; 
        for(int i = 0; i < pixelCount; i++)
        {
            //ldrImg[i] = img[i];
            ldrImg[i] = int(clamp(img[i], 0.f, 255.f));
        }
        write_png(camera.image_name.c_str() , nx, ny, ldrImg);
        std::cout << "writing to file: " << camera.image_name << std::endl;
    }

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


            vec3f color = getRayColor(ray, max_recursion_depth, true, arg->initialMedium, *(arg->camera));
            //vec3i c = clamp(vec3i(color), 0, 255);
            vec3f c = color;

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
    for(int i = 0; i < x; ++i)
    {
        for(int j = 0; j < y; ++j)
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
                ray.type = RAY_TYPE_PRIMARY_RAY;
                ray.throughput = 1.f;
                ray.depth = max_recursion_depth;
                //std::cout << "ray time: " << ray.time << std::endl;
                //vec3f sampleColor = getRayColor(ray, max_recursion_depth, true, arg->initialMedium);
                //vec3f sampleColor = getRayColor(ray, max_recursion_depth, true, arg->initialMedium,  -arg->w, arg->camera->near_distance);
                vec3f sampleColor = getRayColor(ray, max_recursion_depth, true, arg->initialMedium, *(arg->camera));

                sampleColors.push_back(sampleColor);
            }
            
            //apply box filter to samples
            vec3f pixelColor = vec3f(0.f);
            //pixelColor = filterBox(sampleColors); 
            pixelColor = filterGauss(samples, sampleColors); 
            //end box filter
            
            //vec3i c = vec3i(pixelColor);
            vec3f c = vec3f(pixelColor);
            //vec3i c = clamp(vec3i(pixelColor), 0, 255);

            arg->img[(arg->nx*y + x)*3] = c.r;
            arg->img[(arg->nx*y + x)*3 + 1] = c.g;
            arg->img[(arg->nx*y + x)*3 + 2] = c.b;
        }
        y = arg->rows->getNextAvaliableRow();
    }
    

}
