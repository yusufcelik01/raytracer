#ifndef __VEC4__DEFINED__
#define __VEC4__DEFINED__

class vec4f
{
    public:
    union
    {
        struct{float x, y, z, w;};
        struct{float r, g, b, a;};
        struct{float s, t, p, q;};
    };
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
