#ifndef __IMAGE__LIB__
#define __IMAGE__LIB__

#include "ppm.h"
#include "stb_image_write.h"
#include "stb_image.h"
#include "tinyexr.h"

int write_png(char const *filename, int w, int h, const void *data);

#endif
