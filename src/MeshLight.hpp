#ifndef __MESH_LIGHT_DEFINED__
#define __MESH_LIGHT_DEFINED__

#include "Mesh.hpp"

class MeshLight : public Mesh
{
    public:
    Spectrum radiance;
    UniformRandomGenerator* rng;
    
    MeshLight();
    MeshLight(const MeshLight& rhs);
    ~MeshLight();
    float totalSurfaceArea;
    std::vector<float> triangleProbs;

    int sampleIlluminationPoint(const VertexBuffers& vertexBuffers, vec3f shadedPoint, vec3f& Lpoint, vec3f& Lnormal, float& Larea, float& Lprob);
    void preprocess(const VertexBuffers& vertexBuffers);
    private:

    int getRandomFace(int start, int end, float xi);
};


#endif
