#include "ImageTexture.hpp"
#include "rtmath.hpp"

ImageTexture::ImageTexture()
{
    interpolationType = INTERPOLATE_NEAREST;
    normalizer = 255.f;
}


vec3f ImageTexture::sampleNearest(float s, float t)
{
    int x = img.width  * s + 0.5;
    int y = img.height * t + 0.5;

    return img.getRGBpixel(x, y)/normalizer;
    //return img.getRGBpixel(x, y);
}

vec3f ImageTexture::sampleBilinear(float s, float t)
{
    int x = int(img.width  * s);
    int y = int(img.height * t);

    float dx = (img.width * s) - x;
    float dy = (img.height * t) - y;
    
    vec3f c00, c01, c10, c11;

    c00 = img.getRGBpixel(x    , y    );
    c10 = img.getRGBpixel(x + 1, y    );
    c01 = img.getRGBpixel(x    , y + 1);
    c11 = img.getRGBpixel(x + 1, y + 1);


    return mix(mix(c00, c10, dx),
               mix(c01, c11, dx), dy) /normalizer;
}


vec3f ImageTexture::sample(float x, float y)
{
    x = x > 1.0f ? 1.0f : x;
    y = y > 1.0f ? 1.0f : y;
    x = x < 0.0f ? 0.0f : x;
    y = y < 0.0f ? 0.0f : y;

    //return sampleNearest(x, y);
    vec3f sampleColor; 
    if(interpolationType == INTERPOLATE_NEAREST) 
    {
        sampleColor = sampleNearest(x, y);
    }
    else if(interpolationType == INTERPOLATE_BI_LINEAR)
    {
        sampleColor = sampleBilinear(x, y);
    }
    else { 
        sampleColor = vec3f(160, 30, 190);
    }
    return sampleColor;
}


vec3f ImageTexture::sampleDu(float x, float y)
{
    return sample(x + 1.f/img.width, y);
}

vec3f ImageTexture::sampleDv(float x, float y)
{
    return sample(x, y + 1.f/img.height);
}



