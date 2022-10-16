#ifndef __VEC3__DEFINED__
#define __VEC3__DEFINED__

struct vec3f
{
    public:
    union
    {
        struct{float x, y, z;};
        struct{float r, g, b;};
        struct{float s, t, p;};
    };

    vec3f();
    vec3f(float);
    vec3f(float, float, float);
    vec3f operator-();
    vec3f operator-(const vec3f& rhs) const;
    vec3f operator+(const vec3f& rhs);
    vec3f operator*(float& c);
};

struct vec3i
{
    public:
    union
    {
        struct{int x, y, z;};
        struct{int r, g, b;};
        struct{int s, t, p;};
    };

    vec3i();
    vec3i(vec3f vf);
};

vec3f cross(const vec3f& a, const vec3f& b);
float dot(const vec3f& a, const vec3f& b);
float length(const vec3f& a);
vec3f norm(const vec3f& a);
vec3i clamp(const vec3i a);


vec3f operator*(float& c, const vec3f& v);

#endif
