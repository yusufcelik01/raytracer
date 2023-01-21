#include "mat4x4.hpp"
#include <cmath>


mat4x4::mat4x4()
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            m[i][j] = 0.f;
        }
    }
    
    m[0][0] = 1.f;
    m[1][1] = 1.f;
    m[2][2] = 1.f;
    m[3][3] = 1.f;
    
}

mat4x4::mat4x4(const float d)
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            m[i][j] = 0.f;
        }
    }
    
    m[0][0] = d;
    m[1][1] = d;
    m[2][2] = d;
    m[3][3] = d;
}


mat4x4::mat4x4(const mat4x4& rhs)
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            m[i][j] = rhs.m[i][j];
        }
    }
}

mat4x4 mat4x4::operator*(const mat4x4& rhs) const
{
    mat4x4 res(0.f);

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            float sum = 0.f;
            for(int k = 0; k < 4; k++) {
                sum += m[i][k] * rhs.m[k][j];
            }
            res.m[i][j] = sum;
        }
    }

    return res;
}

mat4x4 mat4x4::operator-(const mat4x4& rhs) const
{
    mat4x4 ret(0.f);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            ret.m[i][j] = this->m[i][j] - rhs.m[i][j];
        }
    }
    return ret;
}
mat4x4 mat4x4::operator+(const mat4x4& rhs) const
{
    mat4x4 ret(0.f);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            ret.m[i][j] = this->m[i][j] + rhs.m[i][j];
        }
    }
    return ret;
}

mat4x4 operator*(float c, mat4x4 A)
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            A.m[i][j] *= c;
        }
    }
    return A;
}

mat4x4 mat4x4::operator*(float c) const
{
    mat4x4 ret(*this);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            ret.m[i][j] *= c;
        }
    }
    return ret;
}

mat4x4 mat4x4::operator/(float c) const
{
    mat4x4 ret(*this);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            ret.m[i][j] /= c;
        }
    }
    return ret;
}

vec4f mat4x4::operator*(const vec4f& v) const
{
    vec4f res;
    float tmp[4];
    for(int i=0; i<4; i++)
    {
       tmp[i] = m[i][0]* v.x + 
                m[i][1]* v.y +
                m[i][2]* v.z + 
                m[i][3]* v.w; 
    }
    res.x = tmp[0];
    res.y = tmp[1];
    res.z = tmp[2];
    res.w = tmp[3];
    return res;
}



mat4x4 translate(float x, float y, float z)
{
    mat4x4 T(1.f);

    T.m[0][3] = x;
    T.m[1][3] = y;
    T.m[2][3] = z;

    return T;
}

mat4x4 translate(vec3f v)
{
    mat4x4 T(1.f);

    T.m[0][3] = v.x;
    T.m[1][3] = v.y;
    T.m[2][3] = v.z;

    return T;

}


mat4x4 scale(float x, float y, float z)
{
    mat4x4 S(1.f);

    S.m[0][0] = x;
    S.m[1][1] = y;
    S.m[2][2] = z;

    return S;
}

mat4x4 rotate(float angle, vec3f axis)
{
    angle *= M_PI / 180.f;//convert to radians
    float cosA = cos(angle);
    float sinA = sin(angle);
    float K = 1.f - cosA;
    vec3f n = norm(axis);
    float a = n.x;
    float b = n.y;
    float c = n.z;


    mat4x4 M(0.f);
    M.m[0][0] = cosA + a*a*K;
    M.m[0][1] = a*b*K - c*sinA;
    M.m[0][2] = a*c*K + b*sinA;
    M.m[0][3] = 0.f;

    M.m[1][0] = a*b*K + c*sinA;
    M.m[1][1] = cosA + b*b*K;
    M.m[1][2] = b*c*K - a*sinA;
    M.m[1][3] = 0.f;

    M.m[2][0] = a*c*K - b*sinA;
    M.m[2][1] = b*c*K + a*sinA;
    M.m[2][2] = cosA + c*c*K;
    M.m[2][3] = 0.f;

    M.m[3][0] = 0.f;
    M.m[3][1] = 0.f;
    M.m[3][2] = 0.f;
    M.m[3][3] = 1.f;

    return M;
}


float det(const mat4x4& m)
{
    float a, b, c,
          d, e, f,
          g, h ,i;

    float acc = 0.f;
    //acc = a*e*i + b*f*g + c*h*d - c*e*g - b*d*i - a*f*g;

    a = m.m[1][1]; b = m.m[1][2]; c = m.m[1][3]; 
    d = m.m[2][1]; e = m.m[2][2]; f = m.m[2][3];
    g = m.m[3][1]; h = m.m[3][2]; i = m.m[3][3];
    acc += m.m[0][0] * (a*e*i + b*f*g + c*h*d - c*e*g - b*d*i - a*f*g);
    
    a = m.m[0][1]; b = m.m[0][2]; c = m.m[0][3]; 
    d = m.m[2][1]; e = m.m[2][2]; f = m.m[2][3];
    g = m.m[3][1]; h = m.m[3][2]; i = m.m[3][3];
    acc -= m.m[1][0] * (a*e*i + b*f*g + c*h*d - c*e*g - b*d*i - a*f*g);

    a = m.m[0][1]; b = m.m[0][2]; c = m.m[0][3]; 
    d = m.m[1][1]; e = m.m[1][2]; f = m.m[1][3];
    g = m.m[3][1]; h = m.m[3][2]; i = m.m[3][3];
    acc += m.m[2][0] * (a*e*i + b*f*g + c*h*d - c*e*g - b*d*i - a*f*g);

    a = m.m[0][1]; b = m.m[0][2]; c = m.m[0][3]; 
    d = m.m[1][1]; e = m.m[1][2]; f = m.m[1][3];
    g = m.m[2][1]; h = m.m[2][2]; i = m.m[2][3];
    acc -= m.m[3][0] * (a*e*i + b*f*g + c*h*d - c*e*g - b*d*i - a*f*g);

    return acc;
}


float trace(const mat4x4& m)
{
    float tr = 0.f;
    for(int i = 0; i < 4; ++i) {
        tr += m.m[i][i];
    }
    return tr;
}

mat4x4 inverse(const mat4x4& A) 
{
    float detA = det(A);
    mat4x4 AA = A * A;
    mat4x4 AAA = AA * A;

    float trA = trace(A);
    mat4x4 I = mat4x4(1.f);

    //this is for 3x3 matrices
    //return ( 0.5*(trA*trA - trace(AA))* I - A * trA + AA)/detA;
    return (1.f/6.f*(trA*trA*trA - 3*trA*trace(AA) + 2*trace(AAA) )*I 
             - 0.5*A*(trA*trA - trace(AA)) + AA * trA -AAA) / detA;

}


mat4x4 transpose(const mat4x4& A)
{
    mat4x4 trans;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            trans.m[j][i] = A.m[i][j];
        }
    }

    return trans;
}

std::ostream& operator<<(std::ostream& os, const mat4x4& M)
{
    os << "-----MATRIX-----" << std::endl;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            os << M.m[i][j] << "  ";
        }
        os << '\n';
    }
    os << "----------------" << std::endl;
    return os;
}
