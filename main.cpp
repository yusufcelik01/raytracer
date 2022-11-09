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
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    //force all the objects to calculate their bboxes 
    //so that we dont have to compute bboxes for each comparison
    for(Mesh* mesh : scene.meshes)
    {
        for(auto it = mesh->faces.begin(); it < mesh->faces.end(); ++it)
        {
            (*it)->getBoundingBox(scene.vertex_data);
        }
        mesh->AccBVH = constructMeshBVH(scene.vertex_data, mesh->faces, X_AXIS);
    }
    for(auto it = scene.objects.begin(); it < scene.objects.end(); ++it)
    {
        (*it)->getBoundingBox(scene.vertex_data);
    }
    //scene.AccStrBVH = constructObjectBVH(scene.vertex_data, scene.objects, X_AXIS);

    for(Camera camera: scene.cameras)
    {
        scene.render(camera);
    }
}

