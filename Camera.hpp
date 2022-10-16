#ifndef __CAMERA__
#define __CAMERA__

#include "rtmath.hpp"

struct Camera
{
    vec3f position;
    vec3f gaze;
    vec3f up;
    vec4f near_plane;
    float near_distance;
    int image_width, image_height;
    std::string image_name;
};

#endif
