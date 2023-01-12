#ifndef __CAMERA__
#define __CAMERA__

#include "rtmath.hpp"

enum ToneMappingOperatorType
{
    TMO_PHOTOGRAPHIC_GLOBAL
};

struct ToneMapParamType
{
    enum ToneMappingOperatorType TMO;
    float keyValue;
    float burnPercent;
    float saturation;
    float gamma;
};

struct Camera
{
    vec3f position;
    vec3f gaze;
    vec3f up;
    vec4f near_plane;
    float near_distance;
    int image_width, image_height;
    std::string image_name;

    int numSamples;
    float focusDistance;
    float apertureSize;
    struct ToneMapParamType TMOArgs;
};

#endif
