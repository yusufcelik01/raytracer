#ifndef __VEC2__DEFINED__
#define __VEC2__DEFINED__

class vec2
{
    public:
    union
    {
        struct{float x, y;};
        struct{float r, g;};
        struct{float s, t;};
    };
};


#endif
