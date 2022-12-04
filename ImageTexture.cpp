#include "ImageTexture.hpp"

ImageTexture::ImageTexture()
{
    interpolationType = INTERPOLATE_NEAREST;
}


vec3f ImageTexture::sampleNearest(float s, float t)
{
    int x = img.width  * s + 0.5;
    int y = img.height * t + 0.5;
    //x = 324;
    //y = 524;

    vec3f color;
    color.r =  img.data[(img.width * y + x)*img.numOfChannels    ];
    color.g =  img.data[(img.width * y + x)*img.numOfChannels + 1];
    color.b =  img.data[(img.width * y + x)*img.numOfChannels + 2];

    return color;
}


vec3f ImageTexture::sample(float x, float y)
{
    return sampleNearest(x, y);
    if(interpolationType == INTERPOLATE_NEAREST) 
    {
        return sampleNearest(x, y);
    }
    else if(interpolationType == INTERPOLATE_BI_LINEAR)
    {
        //TODO implement LINEAR INTERPOLATION
    }
    else { 
        return vec3f(160, 30, 190);
    }
}





