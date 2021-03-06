#include "parser.h"
#include "raytracer_math.h"
#include <cmath>
#include <iostream>

parser::Vec3f vectorScalerMult(float c, parser::Vec3f v)
{
    v.x *= c;
    v.y *= c;
    v.z *= c;

    return v;
}

//vector summation
parser::Vec3f vectorSum(parser::Vec3f a, parser::Vec3f b)
{
    parser::Vec3f result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    
    return result;
}

parser::Vec3i vectorSum(parser::Vec3i a, parser::Vec3i b)
{
    parser::Vec3i result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    
    return result;
}
//dot product

float dotProduct(parser::Vec3f v, parser::Vec3f u)
{
    float sum = 0;

    sum += v.x * u.x;
    sum += v.y * u.y;
    sum += v.z * u.z;

    return sum;
}

int dotProduct(parser::Vec3i v, parser::Vec3i u)
{
    int sum = 0;

    sum += v.x * u.x;
    sum += v.y * u.y;
    sum += v.z * u.z;

    return sum;
}


//cross product

parser::Vec3f crossProduct(parser::Vec3f a, parser::Vec3f b)
{
    parser::Vec3f result;

    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;

    return result;
}
parser::Vec3i crossProduct(parser::Vec3i a, parser::Vec3i b)
{
    parser::Vec3i result;

    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;

    return result;
}



parser::Vec3f normalize(const parser::Vec3f& v)// normalizes a vector
{
    float length = VECTOR_LENGTH(v);
    parser::Vec3f n;

    n.x =  v.x/length;
    n.y =  v.y/length;
    n.z =  v.z/length;

    return n;
}

parser::Vec3f elementViseMultiply(const parser::Vec3f& a, const parser::Vec3f& b)
{
    parser::Vec3f res;

    res.x = a.x * b.x;
    res.y = a.y * b.y;
    res.z = a.z * b.z;
    
    return res;
}

parser::Vec3f clampColor(const parser::Vec3f& color)
{
    parser::Vec3f res;

    res.x = (color.x <0)? 0:
       (color.x > 255) ?255 : color.x ;
    res.y = (color.y <0)? 0:
       (color.y > 255) ?255 : color.y ;
    res.z = (color.z <0)? 0:
       (color.z > 255) ?255 : color.z ;
    return res;
}

//operator overloads

parser::Vec3f operator+(const parser::Vec3f& a, const parser::Vec3f& b)
{
    parser::Vec3f temp;

    temp.x = a.x + b.x;
    temp.y = a.y + b.y;
    temp.z = a.z + b.z;

    return temp;
}

parser::Vec3f operator+=(parser::Vec3f& a, const parser::Vec3f& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;

    return a;
}

parser::Vec3f operator-(const parser::Vec3f& a, const parser::Vec3f& b)
{
    parser::Vec3f temp;

    temp.x = a.x - b.x;
    temp.y = a.y - b.y;
    temp.z = a.z - b.z;

    return temp;
}
parser::Vec3f operator-(parser::Vec3f a)
{
    a.x = -a.x; 
    a.y = -a.y; 
    a.z = -a.z; 
    return a;
}

parser::Vec3f operator-=(parser::Vec3f& a, const parser::Vec3f& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;

    return a;
}


parser::Vec3f operator*(const float& a, const parser::Vec3f& b)
{
    parser::Vec3f temp;

    temp.x = a * b.x;
    temp.y = a * b.y;
    temp.z = a * b.z;

    return temp;
}

parser::Vec3f operator/(const parser::Vec3f& vector, const float& c)
{
    parser::Vec3f temp;

    temp.x = vector.x / c;
    temp.y = vector.y / c;
    temp.z = vector.z / c;

    return temp;
}
std::ostream& operator<<(std::ostream& out, const parser::Vec3f& v)
{
    out << '(' << v.x <<", " << v.y << ", " << v.z << ')';

    return out;
}

