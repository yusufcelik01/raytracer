#ifndef __PERLIN__NOISE__DEFINED__
#define __PERLIN__NOISE__DEFINED__

#include "Texture.hpp"
#include <string>

enum noiseConversionType
{
    NOISE_CONV_LINEAR,
    NOISE_CONV_ABSVAL
};


class PerlinNoise: public Texture
{
    private:
        float noiseScale;
        enum noiseConversionType noiseConv;
        //color map
        //perlin layers here
    public:
        PerlinNoise();
        PerlinNoise(float scale, const std::string& conversion);
        vec3f sample(float x, float y);
        vec3f sample(float x, float y, float z);
        vec3f sample(vec3f texCoords);
};


#endif
