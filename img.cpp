#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int write_png(char const *filename, int w, int h, const void *data)
{
    const int comp = 3;
    return stbi_write_png(filename, w, h, comp, data, sizeof(unsigned char) * comp * w);
}




