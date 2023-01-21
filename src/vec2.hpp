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
        struct{float u, v;};
    };

    vec2f();
    vec2f(float X);
    vec2f(const vec2f& rhs);
    vec2f(float X, float Y);
    vec2f operator-(const vec2f& rhs) const;
    vec2f operator+(const vec2f& rhs) const;
    vec2f operator*(const float& c) const;
};

vec2f operator*(const float& c, const vec2f& v);

#endif
