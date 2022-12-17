#ifndef __MATRIX__GENERAL__DEFINED
#define __MATRIX__GENERAL__DEFINED

#include "vec2.hpp"
#include "vec3.hpp"

class Matrix
{
    //private:
    public:
        size_t rows;
        size_t columns;
        float** data;
        Matrix();
        ~Matrix();
        Matrix(size_t r, size_t c);
        Matrix(const Matrix& rhs);

        vec2f getSize() const;
        Matrix operator*(const Matrix& rhs) const;
        Matrix& operator=(const Matrix& rhs);
        Matrix operator*(float c) const;
        vec3f operator*(const vec3f& v) const;
        vec3f convertToVec3();
        Matrix inverse() const;
};


#endif

