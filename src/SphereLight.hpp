#ifndef __SPHERE_LIGHT_DEFINED__ 
#define __SPHERE_LIGHT_DEFINED__

#include "Sphere.hpp"
#include "UniformRandomGenerator.hpp"

class SphereLight: public Sphere
{
    public:
    vec3f radiance;
    UniformRandomGenerator* rng;

    SphereLight();
    SphereLight(const SphereLight& rhs);
    ~SphereLight();

    SampledPoint sampleIlluminationPoint(const VertexBuffers& vertexBuffers, vec3f shadedPoint);
};

#endif
