#ifndef __IMAGE__CLASS__DEFINED__
#define __IMAGE__CLASS__DEFINED__

#include "vec3.hpp"

class Image
{
    public:
        int width, height;
        int numOfChannels;
        unsigned char* data;    
        vec3f getRGBpixel(int x, int y);
};

#endif
