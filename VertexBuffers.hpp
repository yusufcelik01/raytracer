#ifndef __VERTEX__BUFFERS__DEFINED__
#define __VERTEX__BUFFERS__DEFINED__

#include "vec2.hpp"
#include "vec3.hpp"
#include <vector>

class VertexBuffers
{
    public:
    std::vector<vec3f> vertexCoords;
    std::vector<vec2f> textureCoords;
    float epsilon;
};

#endif
