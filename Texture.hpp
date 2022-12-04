#ifndef __TEXTURE__DEFINED__
#define __TEXTURE__DEFINED__
#include "vec3.hpp"

enum tex_mode_t
{
    TEX_MODE_REPLACE_KD,
    TEX_MODE_BLEND_KD,
    TEX_MODE_REPLACE_KS,
    TEX_MODE_REPLACE_BACKGROUND,
    TEX_MODE_REPLACE_NORMAL,
    TEX_MODE_BUMP_NORMAL,
    TEX_MODE_REPLACE_ALL
};


class Texture 
{
    public:
        enum tex_mode_t decalMode;
        float bumpFactor;
        virtual vec3f sample(float x, float y) = 0;
};

#endif
