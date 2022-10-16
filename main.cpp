#include <iostream>
#include "parser.h"
#include "ppm.h"

typedef unsigned char RGB[3];

int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    for(Camera camera: scene.cameras)
    {
        scene.render(camera);
    }
}

