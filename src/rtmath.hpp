#ifndef __RAY__TRACER__MATH__
#define __RAY__TRACER__MATH__

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat4x4.hpp"
#include "UniformRandomGenerator.hpp"

#define FLOAT_MAX std::numeric_limits<float>::max()
#define FLOAT_MIN std::numeric_limits<float>::min()

struct SampledPoint
{
    vec3f point;
    float prob;
};


template <class T>
T mix(T x, T y, float a);

float clamp(float x, float min, float max);


#endif
