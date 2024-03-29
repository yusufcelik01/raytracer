#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "BVHConstruction.hpp"

typedef unsigned char RGB[3];

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cerr << "ERR: missing scene file argument" << std::endl;
    }
    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    //force all the objects to calculate their bboxes 
    //so that we dont have to compute bboxes for each comparison
    for(Mesh* mesh : scene.meshes)
    {
        for(auto it = mesh->faces.begin(); it < mesh->faces.end(); ++it)
        {
            (*it)->getBoundingBox(scene.VAO.vertexCoords);
            (*it)->cacheData(scene.VAO);
        }
        mesh->AccBVH = constructMeshBVH(scene.VAO.vertexCoords, mesh->faces.begin(), mesh->faces.end(), X_AXIS);
        //mesh->bbox = mesh->AccBVH->getBoundingBox();
    }
    for(MeshLight* meshLight : scene.mesh_lights)
    {
        meshLight->preprocess(scene.VAO);
    }

    for(Triangle* triangle : scene.triangles)
    {
        triangle->indices.cacheData(scene.VAO);
    }

    //std::cout << "Mesh BVHs are completed" << std::endl;
    for(auto it = scene.objects.begin(); it < scene.objects.end(); ++it)
    {
        (*it)->getBoundingBox(scene.VAO.vertexCoords);
    }
    scene.AccStrBVH = constructObjectBVH(scene.VAO.vertexCoords, scene.objects.begin(), scene.objects.end(), X_AXIS);
    std::cout << "BVH is completed" << std::endl;

    for(Camera camera: scene.cameras)
    {
        scene.render(camera);
    }

    //delete scene.AccStrBVH;
        
    return 0;
}

