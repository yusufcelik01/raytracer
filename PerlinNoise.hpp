#ifndef __PERLIN__NOISE__DEFINED__
#define __PERLIN__NOISE__DEFINED__

#include "Texture.hpp"

class PerlinNoise: public Texture
{
    private:
        float noiseScale;
        float noiseConv;
        //color map
        //perlin layers here
    public:
        sample(float x, float y);
        sample(float x, float y, float z);
};


#endif
