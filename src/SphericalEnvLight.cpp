#include "SphericalEnvLight.hpp"
#include <cmath>

vec3f SphericalEnvLight::sample(vec3f dir)
{
    float u = (1.f + atan2f(dir.x, -dir.z)/M_PI)/2.f; 
    float v = acos(dir.y)/M_PI;

    return tex.sample(u, v);
}
