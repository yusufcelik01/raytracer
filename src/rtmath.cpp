#include "rtmath.hpp"

template <class T>
T mix(T x, T y, float a)
{
    return x * (1 - a) + y * a;
}

float clamp(float x, float min, float max)
{
    x = min > x ? min : x;
    x = max < x ? max : x;
    return x;
}

float clamp(float x, float min, float max);


template vec3f mix<vec3f>(vec3f, vec3f, float);
template float mix<float>(float, float, float);

//returns a random vector sampled from a hemisphere
//such that the z(0,0,1) is the vector aligned with hemispheres up vector
//xi1 and xi2 two must be in range [0.f, 1.f]

vec3f uniformSampleHemiSphere(float xi1, float xi2)
{
    float z = xi1; 
    float r = std::sqrt(std::max(0.f, 1.f - z*z));//r = sinTheta
    float phi = 2 * M_PI * xi2;

    return vec3f(r * cos(phi), r * sin(phi), z);
}

//same logic but sampled with cosine weight towards upper hemisphere
vec3f cosineSampleHemiSphere(float xi1, float xi2)
{
    vec2f d = uniformSampleDisc(xi1, xi2);
    float z = std::sqrt(std::max(0.f, 1.f - d.x * d.x - d.y * d.y));
    return vec3f(d.x, d.y, z);
}

float cosineHemispherePdf(float cosTheta)
{
    return cosTheta / M_PI;
}

vec2f uniformSampleDisc(float xi1, float xi2)
{
    float r = std::sqrt(xi1);
    float theta = 2 * M_PI * xi2;
    return vec2f(r * std::cos(theta), r * std::sin(theta));
}
