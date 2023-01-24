#ifndef __MESH_LIGHT_DEFINED__
#define __MESH_LIGHT_DEFINED__

#include "Mesh.hpp"

class MeshLight : public Mesh
{
    public:
    vec3f radiance;
    UniformRandomGenerator* rng;
    
    MeshLight();
    MeshLight(const MeshLight& rhs);
    ~MeshLight();
    float totalSurfaceArea;
    std::vector<float> triangleProbs;

    SampledPoint sampleIlluminationPoint(const VertexBuffers& vertexBuffers, vec3f shadedPoint);
    void preprocess(const VertexBuffers& vertexBuffers);
};


#endif
