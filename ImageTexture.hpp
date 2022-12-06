#ifndef __IMAGE__TEXTURE__DEFINED__
#define __IMAGE__TEXTURE__DEFINED__

#include "Texture.hpp"
#include "Image.hpp"

enum  TexSamplerInterpolatorType
{
    INTERPOLATE_BI_LINEAR,
    INTERPOLATE_NEAREST
};


class ImageTexture: public Texture
{
    private:
        vec3f sampleBilinear(float s, float t);
        vec3f sampleNearest(float s, float t);
    public:
        TexSamplerInterpolatorType interpolationType;
        Image img; 
        ImageTexture();
        vec3f sample(float x, float y);
};


#endif
