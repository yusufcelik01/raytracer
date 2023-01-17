#ifndef __TEXTURE__DEFINED__
#define __TEXTURE__DEFINED__
#include "vec3.hpp"
#include <iostream>

#define TEX_SAMPLING_EPSILON 0.0001

enum tex_mode_t
{
    TEX_MODE_REPLACE_KD,
    TEX_MODE_BLEND_KD,
    TEX_MODE_REPLACE_KS,
    TEX_MODE_REPLACE_MIRROR,
    TEX_MODE_SCALE_MIRROR,
    TEX_MODE_REPLACE_BACKGROUND,
    TEX_MODE_REPLACE_NORMAL,
    TEX_MODE_BUMP_NORMAL,
    TEX_MODE_REPLACE_ALL,
    TEX_MODE_ENV_LIGHT
};

enum TexSamplerType
{
    TEX_SAMPLER_1D,
    TEX_SAMPLER_2D,
    TEX_SAMPLER_3D,
    TEX_SAMPLER_CUBE
};

class Texture 
{
    public:
        enum tex_mode_t decalMode;
        enum TexSamplerType samplerType;
        float bumpFactor;
        Texture(){ 
            decalMode = TEX_MODE_REPLACE_KD;
            samplerType = TEX_SAMPLER_2D; bumpFactor = 1.f;}

        virtual vec3f sample(float x, float y) = 0;
        virtual vec3f sample(vec3f texCoord) 
            { 
                return sample(texCoord.x, texCoord.y);}
        virtual vec3f sampleDu(float x, float y)
            { return sample(x+ TEX_SAMPLING_EPSILON,y);}
        virtual vec3f sampleDv(float x, float y)
            { return sample(x,y + TEX_SAMPLING_EPSILON);}
};

#endif
