#ifndef __VEC4__DEFINED__
#define __VEC4__DEFINED__

#include "vec3.hpp"

class vec4f
{
    public:
    union
    {
        struct{float x, y, z, w;};
        struct{float r, g, b, a;};
        struct{float s, t, p, q;};
    };

    vec4f();
    vec4f(float);
    vec4f(float, float, float, float);
    vec4f(vec3f, float);
};

class vec4i
{
    public:
    union
    {
        struct{int x, y, z, w;};
        struct{int r, g, b, a;};
        struct{int s, t, p, q;};
    };
};


#endif
