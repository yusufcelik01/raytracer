#include "MeshLight.hpp"


MeshLight::MeshLight()
{
    totalSurfaceArea = 0.f;
    radiance = Spectrum(0.f);
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
    triangleProbs[triangleProbs.size()-1] = 1.f;
    //now each element has the cumulative probability 
    //this will help us while doing binary search on the list
}

//return -1 on error
//return 0 on succes 
//write all the other return values to references arguments if no error occurs
//int MeshLight::sampleIlluminationPoint(const VertexBuffers& vertexBuffers, vec3f shadedPoint)
int MeshLight::sampleIlluminationPoint(const VertexBuffers& vertexBuffers, vec3f shadedPoint, vec3f& Lpoint, vec3f& Lnormal, float& Larea, float& Lprob)
{
    //first convert the shaded point to object space
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

    //get a face that can see the point
    Face* sampledFace = NULL; 
    vec3f v1;
    vec3f v2;
    vec3f v3;

        float xi = rng->getUniformRandNumber(0.f, 1.f); 
        int sampledIndex = getRandomFace(0, faces.size() - 1, xi);
        sampledFace = faces[sampledIndex];  
        v1 = vertexBuffers.vertexCoords[sampledFace->vertexId[0]];
        v2 = vertexBuffers.vertexCoords[sampledFace->vertexId[1]];
        v3 = vertexBuffers.vertexCoords[sampledFace->vertexId[2]];
    //int startIndex = 0;
    //int endIndex = faces.size() - 1;
    //float startProb = 0.f;
    //float endProb = 1.f;
    //while(true)
    //{
    //    float xi = rng->getUniformRandNumber(startProb, endProb); 
    //    int sampledIndex = getRandomFace(startIndex, endIndex, xi);
    //    sampledFace = faces[sampledIndex];  

    //    v1 = vertexBuffers.vertexCoords[sampledFace->vertexId[0]];
    //    v2 = vertexBuffers.vertexCoords[sampledFace->vertexId[1]];
    //    v3 = vertexBuffers.vertexCoords[sampledFace->vertexId[2]];

    //    vec3f center = (v1 + v2 + v3) / 3.f;
    //    vec3f w_i = norm(center - pLocal); 

    //    if(dot(w_i, *(sampledFace->normal)) <= 0.f)//if NOT backface
    //    {
    //        break;//if bot backface
    //    }
    //    else
    //    {
    //        if(startIndex == sampledIndex) {
    //            startProb = triangleProbs[startIndex];
    //            startIndex++;
    //        }
    //        else if(endIndex == sampledIndex) {
    //            endProb = triangleProbs[endIndex -1];
    //            endIndex--;
    //        }
    //        if(startIndex > endIndex)
    //        {
    //            break;//no front facing triangles
    //        }

    //    }
    //}
    
    //TODO sample point on triangle
    float xi1 = rng->getUniformRandNumber(0.f, 1.f); 
    float xi2 = rng->getUniformRandNumber(0.f, 1.f); 
    vec3f q =  xi1 * v1 + (1.f - xi1) * v2;
    float sqrtXi = sqrt(xi2);
    vec3f p = sqrtXi * q + (1.f - sqrtXi) * v3; 

    //transfrom them back to world coords
    tmp = compositeTransformation * vec4f(p, 1.f);
    vec3f worldLightPoint(tmp.x, tmp.y, tmp.z);

    tmp =  transpose(invM) * vec4f(*(sampledFace->normal), 0.f);
    
    Larea = sampledFace->area;
    Lprob = sampledFace->area / totalSurfaceArea;
    Lnormal = norm(vec3f(tmp.x, tmp.y, tmp.z));
    Lpoint = worldLightPoint;

    return 0;
}

int MeshLight::getRandomFace(int start, int end, float xi)
{
    if(start > end)
    {
        //std::cout << "returning WITH ERROR: " << std::endl;
        return -1;
    }
    else if(end == start)
    {
        //std::cout << "returning random face with index: " << end << std::endl;
        return end;
    }

    int mid = (end + start)/2;
    if(xi > triangleProbs[mid])
    {
        return getRandomFace(mid+1, end, xi);
    }
    else// xi <= mid
    {
        if(mid > 0)
        {
            if(triangleProbs[mid-1] < xi)
            {
                return mid;    
            }
        }
        else// mid == 0
        {
            return 0;
        }
        return getRandomFace(start, mid-1, xi);
    }
}
