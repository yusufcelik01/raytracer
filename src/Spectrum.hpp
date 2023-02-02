#ifndef __SPECTRUM_DEFINED__
#define __SPECTRUM_DEFINED__

#include <cmath>
#include "vec3.hpp"

static const int lambdaStart = 380;
static const int lambdaEnd = 700;
static const int SpectralSampleCount = 60;


enum class SpectrumType {
    Reflectance,
    Illuminant
};


template <int nSamples>
class CoefficientSpectrum
{
    protected:
        float c[nSamples];
    public:
        CoefficientSpectrum(float v = 0.f) {
            for(int i = 0; i < nSamples; ++i){
                c[i] = v;
            }
        }

        CoefficientSpectrum(const CoefficientSpectrum& rhs) {
            for(int i = 0; i < nSamples; ++i){
                c[i] = rhs.c[i];
            }
        }

        CoefficientSpectrum hasNaNs() {
            for(int i = 0; i < nSamples; ++i){
                if(c[i] != c[i] ) return true;
            }
        }

        float minCoefVal() {
            float min = c[0];
            for(int i = 1; i < nSamples; ++i){
                min = (min < c[i]) ? min : c[i];
            }
            return min;
        }

        float maxCoefVal() {
            float max = c[0];
            for(int i = 1; i < nSamples; ++i){
                max = (max > c[i]) ? max : c[i];
            }
            return max;
        }

        CoefficientSpectrum operator+(const CoefficientSpectrum& rhs) const{
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] + rhs.c[i];
            }
            return tmp;
        }

        CoefficientSpectrum operator-(const CoefficientSpectrum& rhs) const{
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] - rhs.c[i];
            }
            return tmp;
        }

        CoefficientSpectrum operator*(const CoefficientSpectrum& rhs) const{
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] * rhs.c[i];
            }
            return tmp;
        }

        CoefficientSpectrum operator*(const float& s) const{
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] * s;
            }
            return tmp;
        }

        CoefficientSpectrum operator/(const float& s) const{
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] / s;
            }
            return tmp;
        }

        CoefficientSpectrum operator/(const CoefficientSpectrum& rhs) const{
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] / rhs.c[i];
            }
            return tmp;
        }

        CoefficientSpectrum operator-() const{
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = -c[i];
            }
            return tmp;
        }
        //assignment ops
        CoefficientSpectrum& operator=(const CoefficientSpectrum& rhs){
            for(int i = 0; i < nSamples; ++i){
                c[i] = rhs.c[i];
            }
            return *this;
        }

        CoefficientSpectrum& operator+=(const CoefficientSpectrum& rhs) {
            for(int i = 0; i < nSamples; ++i){
                c[i] += rhs.c[i];
            }
            return *this;
        }

        CoefficientSpectrum& operator-=(const CoefficientSpectrum& rhs) {
            for(int i = 0; i < nSamples; ++i){
                c[i] -= rhs.c[i];
            }
            return *this;
        }

        CoefficientSpectrum& operator*=(const CoefficientSpectrum& rhs) {
            for(int i = 0; i < nSamples; ++i){
                c[i] *= rhs.c[i];
            }
            return *this;
        }

        CoefficientSpectrum& operator/=(const CoefficientSpectrum& rhs) {
            for(int i = 0; i < nSamples; ++i){
                c[i] /= rhs.c[i];
            }
            return *this;
        }

        friend CoefficientSpectrum Sqrt(const CoefficientSpectrum& rhs) {
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = std::sqrt(rhs.c[i]);
            }
            return tmp;
        }

        friend CoefficientSpectrum Exp(const CoefficientSpectrum& rhs) {
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = std::exp(rhs.c[i]);
            }
            return tmp;
        }

        friend CoefficientSpectrum Pow(const CoefficientSpectrum& rhs, float n) {
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = std::pow(rhs.c[i], n);
            }
            return tmp;
        }

        friend CoefficientSpectrum Lerp(float a, 
                                       const CoefficientSpectrum& s1, 
                                       const CoefficientSpectrum& s2) {
            CoefficientSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = (1.f - a) * s1.c[i] + a * s2.c[i];
            }
            return tmp;
        }

        friend CoefficientSpectrum operator*(const float& c, const CoefficientSpectrum& v) {
            CoefficientSpectrum ret(v);
            for(int i = 0; i < nSamples; ++i) {
                ret.c[i] *= c;
            }
            return ret;
        }
   
};



class RGBSpectrum : public CoefficientSpectrum<3> {
    //fromRGB(float r, float g, float b); 
    
    public:
    RGBSpectrum(float v = 0.f) : CoefficientSpectrum<3>(v) {}
    RGBSpectrum(const CoefficientSpectrum<3> &v) : CoefficientSpectrum<3>(v) {}
    
    static RGBSpectrum fromRGB(vec3f rgb, SpectrumType type = SpectrumType::Reflectance) {
        RGBSpectrum s;
        s.c[0] = rgb[0];
        s.c[1] = rgb[1];
        s.c[2] = rgb[2];
        return s;
    }

    vec3f toRGB() const {
        vec3f rgb;
        rgb[0] = c[0]; 
        rgb[1] = c[1]; 
        rgb[2] = c[2]; 
        return rgb;
    }

    static RGBSpectrum fromXYZ(vec3f xyz) {
        RGBSpectrum s;
        s.c[0] = xyz[0];
        s.c[1] = xyz[1];
        s.c[2] = xyz[2];
        return s;
    }

    vec3f toXYZ() const {//TODO this actually not converting
        vec3f xyz;
        xyz[0] = c[0]; 
        xyz[1] = c[1]; 
        xyz[2] = c[2]; 
        return xyz;
    }


};


//#ifdef SPECTRAL_RAY_TRACER
//    typedef SampledSpectrum Spectrum;
//#else 
//    typedef RGBSpectrum Spectrum;
//#endif
//
typedef RGBSpectrum Spectrum;


#endif
