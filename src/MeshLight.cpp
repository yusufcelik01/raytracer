#include "MeshLight.hpp"


MeshLight::MeshLight()
{
    totalSurfaceArea = 0.f;
    radiance = vec3f();
    rng = new UniformRandomGenerator;
}

MeshLight::MeshLight(const MeshLight& rhs) : Mesh(rhs)
{
    radiance = rhs.radiance;
    totalSurfaceArea = rhs.totalSurfaceArea;
    rng = new UniformRandomGenerator;
    triangleProbs = rhs.triangleProbs;
}

MeshLight::~MeshLight()
{
    if(rng)
    {
        delete rng;
    }
}

void MeshLight::preprocess(const VertexBuffers& vertexBuffers)
{
    totalSurfaceArea = 0.f;
    triangleProbs = {};
    for(Face* face : faces)
    {
        float a = face->area;
        totalSurfaceArea += a;
        triangleProbs.push_back(a);
    }

    float oneOverArea = 1.f / totalSurfaceArea;

    for(float& a : triangleProbs)
    {
        a *= oneOverArea;//divide them by total area
    }
    //each element now has their probability
    //now compute the cumulative probability
    float acc = 0.f;
    for(float& a : triangleProbs)
    {
        a += acc;
        acc = a;
    }
    //now each element has the cumulative probability 
    //this will help us while doing binary search on the list
}

SampledPoint MeshLight::sampleIlluminationPoint(const VertexBuffers& vertexBuffers, vec3f shadedPoint)
{

}
