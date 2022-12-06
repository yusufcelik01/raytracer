#include "Image.hpp"

vec3f Image::getRGBpixel(int x, int y)
{
    vec3f color;
    color.r =  data[(width * y + x)*numOfChannels    ];
    color.g =  data[(width * y + x)*numOfChannels + 1];
    color.b =  data[(width * y + x)*numOfChannels + 2];

    return color;
}
