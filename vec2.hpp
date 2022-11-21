#ifndef __VEC2__DEFINED__
#define __VEC2__DEFINED__

class vec2f
{
    public:
    union
    {
        struct{float x, y;};
        struct{float r, g;};
        struct{float s, t;};
    };

    vec2f();
    vec2f(float X);
    vec2f(float X, float Y);
};


#endif
