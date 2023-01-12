#ifndef __IMAGE__CLASS__DEFINED__
#define __IMAGE__CLASS__DEFINED__

#include "vec3.hpp"

enum ImageDataType
{
    UNSIGNED_CHAR,
    FLOAT
};

class Image
{
    public:
        int width, height;
        int numOfChannels;
        unsigned char* data;    
        float* floatData;
        enum ImageDataType dataType;
        vec3f getRGBpixel(int x, int y);
};

#endif
