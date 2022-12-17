#include "Image.hpp"

vec3f Image::getRGBpixel(int x, int y)
{
    if (x > width) x = width -1;
    if (y > height) y = height -1;
    vec3f color;
    color.r =  data[(width * y + x)*numOfChannels    ];
    color.g =  data[(width * y + x)*numOfChannels + 1];
    color.b =  data[(width * y + x)*numOfChannels + 2];

    return color;
}
