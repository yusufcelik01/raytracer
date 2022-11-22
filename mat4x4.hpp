#ifndef __MATRIX__4X4__DEFINED
#define __MATRIX__4X4__DEFINED

#include "vec3.hpp"
#include "vec4.hpp"

class mat4x4
{
    public:
        float m[4][4];

        mat4x4();
        mat4x4(const float);//diagonal matrix
        mat4x4(const mat4x4& );

        mat4x4 operator*(const mat4x4& rhs) const;
        mat4x4 operator*=(const mat4x4& rhs) const;
        vec4f operator*(const vec4f& v) const;
        mat4x4 operator*(float c) const;
        mat4x4 operator/(float c) const;

        mat4x4 operator-(const mat4x4& rhs) const;
        mat4x4 operator+(const mat4x4& rhs) const;

        //mat4x4 inverse() const;
};



mat4x4 translate(float s);
mat4x4 translate(float x, float y, float z);
mat4x4 translate(vec3f);

mat4x4 scale(float s);
mat4x4 scale(float x, float y, float z);

//rotate around major axis'
mat4x4 rotateX(float angle);
mat4x4 rotateY(float angle);
mat4x4 rotateZ(float angle);

mat4x4 rotate(float angle, vec3f axis);

mat4x4 inverse(const mat4x4& m);
mat4x4 transpose(const mat4x4& A);

mat4x4 operator*(float c, mat4x4);
//mat4x4 operator*(const vec3& rhs) const;

#endif
