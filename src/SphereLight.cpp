#include "SphereLight.hpp"

SphereLight::SphereLight()
{
    radiance = vec3f(0.f);
    rng = new UniformRandomGenerator;
}

SphereLight::SphereLight(const SphereLight& rhs)
{
    radiance = rhs.radiance;
    rng = new UniformRandomGenerator;
}

SphereLight::~SphereLight()
{
    if(rng)
    {
        delete rng;
        rng = NULL;
    }
}


SampledPoint SphereLight::sampleIlluminationPoint(const VertexBuffers& vertexBuffers, vec3f shadedPoint)
{
    mat4x4 compositeTransformation(1.f), invM(1.f);

    if(transformation != NULL)
    {
        //std::cout << "transformations are here" << std::endl;
        compositeTransformation = *transformation;
    }
    //if(motionBlur)//for now lights can not move
    //{
    //    compositeTransformation = translate(*motionBlur * r.time) * compositeTransformation;
    //}
    invM = inverse(compositeTransformation);

    //transform the shaded point to object space
    vec4f tmp = invM * vec4f(shadedPoint, 1.f);
    vec3f pLocal(tmp.x, tmp.y, tmp.z);

    vec3f c = vertexBuffers.vertexCoords[center_vertex_id];
    vec3f d = c - pLocal;
    float thetaMax = asin(radius / length(d));

    float xi1 = rng->getUniformRandNumber(0, 1);
    float xi2 = rng->getUniformRandNumber(0, 1);

    float phi = 2 * M_PI * xi1;
    float theta = acos(1.f - xi2 + (xi2 * cos(thetaMax)));

    ONB onb(d);

    float sinTheta = sin(theta);
    vec3f sampleLightDir = onb.n * cos(theta) 
                         + onb.u * sinTheta * cos(phi)
                         + onb.v * sinTheta * sin(phi);

    //intersect ray with sphere to find the light sample point
    float t = -1.f;
    SampledPoint errValue;
    errValue.point = vec3f(std::nan("1"));
    errValue.prob = std::nan("1");
    {
        Ray r;
        r.o = pLocal;
        r.d = sampleLightDir;

        vec3f o_min_c = r.o - c;
        float d_dot__omc = dot(r.d, o_min_c);
        float disc = (d_dot__omc*d_dot__omc) - (dot(r.d, r.d)*(dot(o_min_c, o_min_c) - radius*radius));
        if(disc <= 0)
        {
            return errValue;
            //exit(-1);
            //return false;
        }

        float t1 = (-d_dot__omc + sqrt(disc))/ dot(r.d, r.d);
        float t2 = (-d_dot__omc - sqrt(disc))/ dot(r.d, r.d);

        if(t1 >= 0)
        {
            if(t2 >= 0)
            {
                t = (t1 < t2) ? t1 : t2;
            }
            else
            {
                t = t1;
            }
        }
        else
        {
            if(t2 >= 0)
            {
                t = t2;
            }
            else
            {
                return errValue;
                //return vec3f(std::nan("1"));
                //std::cerr << "sphere light sampling error" << std::endl;
                //exit(-1);
            }
        }
    }
    vec3f lightPoint = pLocal + sampleLightDir * t;
    tmp = compositeTransformation * vec4f(lightPoint, 1.f);
    vec3f worldLightPoint(tmp.x, tmp.y, tmp.z);

    SampledPoint sampledFinalPoint;
    sampledFinalPoint.point = worldLightPoint;
    sampledFinalPoint.prob = 1.f/ ( 2* M_PI * (1.f - cos(thetaMax)));

    //apply transformations back
    return sampledFinalPoint;
}
