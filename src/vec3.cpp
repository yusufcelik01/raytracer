#include "vec3.hpp"
#include <cmath>


vec3f::vec3f()
{
    this->x = 0.f;
    this->y = 0.f;
    this->z = 0.f;
}

vec3f::vec3f(float c)
{
    this->x = c;
    this->y = c;
    this->z = c;
}

vec3f::vec3f(const vec3i& ivec)
{
    this->x = ivec.x;
    this->y = ivec.y;
    this->z = ivec.z;
}

//vec3f::vec3f(const vec3f& vec)
//{
//    this->x = vec.x;
//    this->y = vec.y;
//    this->z = vec.z;
//}

vec3f::vec3f(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3f vec3f::operator-()
{
    vec3f u;
    u.x = -this->x;
    u.y = -this->y;
    u.z = -this->z;

    return u;
}

vec3f vec3f::operator-(const vec3f& rhs) const
{
    vec3f v;
    v.x = this->x - rhs.x;
    v.y = this->y - rhs.y;
    v.z = this->z - rhs.z;

    return v;
}

vec3f vec3f::operator+(const vec3f& rhs) const
{
    vec3f v;
    v.x = this->x + rhs.x;
    v.y = this->y + rhs.y;
    v.z = this->z + rhs.z;

    return v;
}

vec3f cross(const vec3f& a, const vec3f& b)
{
    vec3f c;
    
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;

    return c;
}

float dot(const vec3f& a, const vec3f& b)
{
    float c;
    
    c = a.x * b.x + a.y * b.y + a.z * b.z;

    return c;
}

/*
 all vectors must be unit
 */
vec3f reflect(vec3f n, vec3f incoming)
{
    return -incoming + ( 2.f * n * (dot(n, incoming)));
}

/*
  returns the refracting vector
  if total internal reflection occurs returns a 0 vector
  eta = n1/n2 where n1 is the current mediums refraction index
                and n2 is the refraction index we are about to enter
  
  */
vec3f refract(vec3f n, vec3f incoming, float eta)
{
    float cosTheta = dot(-incoming, n);
    float cosPhi_squared = 1.f - eta*eta * (1.f - cosTheta*cosTheta);
    if( cosPhi_squared < 0)//total internal reflection
    {
        return vec3f(0.f);
    }
    else
    {
        //return norm((incoming + n*cosTheta)*eta - n * sqrt(cosPhi_squared));
        //return eta * (-incoming) - (eta * dot(n, -incoming) + sqrt(cosPhi_squared) ) * n;
        return (incoming + n * cosTheta)*eta - (n * sqrt(cosPhi_squared));
    }
}

float dielectricReflectionRatio(float n1, float n2, float cosTheta, float cosPhi)
{
    float rPar = (n2 * cosTheta - n1 * cosPhi)
                /(n2 * cosTheta + n1 * cosPhi);

    float rPer = (n1 * cosTheta - n2 * cosPhi)
                /(n1 * cosTheta + n2 * cosPhi);

    return 0.5 * (rPar * rPar + rPer * rPer);
}

float conductorReflectionRatio(float n2, float k2, float cosTheta)
{
    float n2_k2 = n2*n2 + k2*k2;
    float twoNcosTheta = 2. * n2 * cosTheta;
    
    float Rs = (n2_k2 - twoNcosTheta + cosTheta*cosTheta)
              /(n2_k2 + twoNcosTheta + cosTheta*cosTheta);
    float Rp = (n2_k2 * cosTheta * cosTheta - twoNcosTheta + 1)
              /(n2_k2 * cosTheta * cosTheta + twoNcosTheta + 1); 

    return 0.5 * (Rs + Rp);
}

float length(const vec3f& a)
{
    float l = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);

    return l;
}

vec3f norm(const vec3f& a)
{
    float k = 1.f/sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
     

    return k * a;
}

//vec3f operator*(const vec3f& v, float& c)
//{
//   vec3f u; 
//   u.x = c * v.x;
//   u.y = c * v.y;
//   u.z = c * v.z;
//
//   return u;
//}
vec3f vec3f::operator*(const float& c) const
{
   vec3f u; 
   u.x = c * x;
   u.y = c * y;
   u.z = c * z;

   return u;
}
vec3f vec3f::operator/(const float& c) const
{
   vec3f u; 
   u.x = x / c;
   u.y = y / c;
   u.z = z / c;

   return u;
}
vec3f operator*(const float& c, const vec3f& v)
{
   vec3f u; 
   u.x = c * v.x;
   u.y = c * v.y;
   u.z = c * v.z;

   return u;
}


/// vec3i member functions
vec3i::vec3i()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}


vec3i::vec3i(vec3f vf)
{
    this->x = int(vf.x);
    this->y = int(vf.y);
    this->z = int(vf.z);
}

vec3i clamp(const vec3i a, vec3i lower, vec3i upper)
{
    vec3i v = a;
    if(v.x > upper.x){
        v.x = upper.x;
    }
    else if(v.x < lower.x){
        v.x = lower.x;
    }
    
    if(v.y > upper.y){
        v.y = upper.y;
    }
    else if(v.y < lower.y){
        v.y = lower.y;
    }

    if(v.z > upper.z){
        v.z = upper.z;
    }
    else if(v.z < lower.z){
        v.z = lower.z;
    }
    return v;
}
vec3i clamp(const vec3i a, int lower, int upper)
{
    vec3i v = a;
    if(v.x > upper){
        v.x = upper;
    }
    else if(v.x < lower){
        v.x = lower;
    }
    
    if(v.y > upper){
        v.y = upper;
    }
    else if(v.y < lower){
        v.y = lower;
    }

    if(v.z > upper){
        v.z = upper;
    }
    else if(v.z < lower){
        v.z = lower;
    }
    return v;
}

vec3f vec3f::operator*(const vec3f& rhs) const
{
    vec3f ret;
    ret.x = x * rhs.x;
    ret.y = y * rhs.y;
    ret.z = z * rhs.z;
    return ret;
}
vec3f& vec3f::operator+=(const vec3f& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}
vec3f& vec3f::operator-=(const vec3f& rhs) 
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

vec3f& vec3f::operator/=(const float c)
{
    x /= c;
    y /= c;
    z /= c;
    return *this;
}
bool vec3f::operator==(const vec3f& rhs) 
{
    if (x == rhs.x 
     && y == rhs.y
     && z == rhs.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float& vec3f::operator[](int index)
{
    if(index == 0){
        return x;
    }
    else if(index == 1){
        return y;
    }
    else if(index == 2){
        return z;
    }
    else{
        return x;
    }
}


vec3f exp(vec3f v)
{
    v.x = exp(v.x);
    v.y = exp(v.y);
    v.z = exp(v.z);
    return v;
}

ONB::ONB()
{
    n = vec3f(1, 0, 0);
    v = vec3f(0, 1, 0);
    u = vec3f(0, 0, 1);
}

ONB::ONB(vec3f N)
{
    n = norm(N);
    vec3f np = n;
    if(abs(np.x) < abs(np.y)) {
        if(abs(np.x) < abs(np.z)) { //npx smallest
            if(np.x < 0) np.x = -1.f;       
            else        np.x = 1.f;
        }
        else { // npz smallest
            if(np.z < 0) np.z = -1.f;       
            else        np.z = 1.f;
        }
    }
    else {//npx > np.y
        if(abs(np.y) < abs(np.z)) { //npy smallest
            if(np.y < 0) np.y = -1.f;       
            else        np.y = 1.f;
        }
        else {//npz smallest
            if(np.z < 0) np.z = -1.f;       
            else        np.z = 1.f;
        }
    }

    u = norm(cross(np, n));
    v = norm(cross(n, u));
}

std::ostream& operator<<(std::ostream& os, const vec3f& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

float makeGrayScale(vec3f rgb)
{
    float sum = 0;
    sum += rgb.r;
    sum += rgb.g;
    sum += rgb.b;
        
    return sum/3.f;
}

vec3f elemWiseExp(vec3f v, float s)
{
    vec3f ret(0.f);

    ret.r = pow(v.r, s);
    ret.g = pow(v.g, s);
    ret.b = pow(v.b, s);

    return ret;
}

float getMaxComp(vec3f v)
{
    return std::max(v.x, std::max(v.y, v.z));
}
