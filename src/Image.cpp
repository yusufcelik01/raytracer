#include "Image.hpp"

vec3f Image::getRGBpixel(int x, int y)
{
    if (x > width) x = width -1;
    if (y > height) y = height -1;
    vec3f color;
    switch(dataType)
    {
        case UNSIGNED_CHAR:
            //color.r =  ((unsigned char*) data)[(width * y + x)*numOfChannels    ];
            //color.g =  ((unsigned char*) data)[(width * y + x)*numOfChannels + 1];
            //color.b =  ((unsigned char*) data)[(width * y + x)*numOfChannels + 2];
            color.r =  data[(width * y + x)*numOfChannels    ];
            color.g =  data[(width * y + x)*numOfChannels + 1];
            color.b =  data[(width * y + x)*numOfChannels + 2];
            break;
        case FLOAT:
            //color.r =  ((float*) data)[(width * y + x)*numOfChannels    ];
            //color.g =  ((float*) data)[(width * y + x)*numOfChannels + 1];
            //color.b =  ((float*) data)[(width * y + x)*numOfChannels + 2];
            color.r =  floatData[(width * y + x)*numOfChannels    ];
            color.g =  floatData[(width * y + x)*numOfChannels + 1];
            color.b =  floatData[(width * y + x)*numOfChannels + 2];
            break;

    }

    return color;
}
