#ifndef __SPECTRUM_DEFINED__
#define __SPECTRUM_DEFINED__

#include <cmath>

//#ifdef SPECTRAL_RAY_TRACER
//    typedef SampledSpectrum Spectrum;
//#else 
//    typedef RGBSpectrum Spectrum;
//#endif
//
typedef RGBSpectrum Spectrum;

//TODO define lambdas and sample count


static const int lambdaStart = 380;
static const int lambdaEnd = 700;
static const int SpectralSampleCount = 60;


enum class SpectrumType {
    Reflectance,
    Illumination
};


template <int nSamples>
class CoefficentSpectrum
{
    protected:
        float c[nSamples];
    public:
        CoefficentSpectrum(float v = 0.f) {
            for(int i = 0; i < nSamples; ++i){
                c[i] = v;
            }
        }

        CoefficentSpectrum(const CoefficentSpectrum& rhs) {
            for(int i = 0; i < nSamples; ++i){
                c[i] = rhs.c[i];
            }
        }

        CoefficentSpectrum hasNaNs() {
            for(int i = 0; i < nSamples; ++i){
                if(c[i] != c[i] ) return true;
            }
        }

        CoefficentSpectrum operator+(const CoefficentSpectrum& rhs) const{
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] + rhs.c[i];
            }
            return tmp;
        }

        CoefficentSpectrum operator-(const CoefficentSpectrum& rhs) const{
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] - rhs.c[i];
            }
            return tmp;
        }

        CoefficentSpectrum operator*(const CoefficentSpectrum& rhs) const{
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] * rhs.c[i];
            }
            return tmp;
        }

        CoefficentSpectrum operator/(const CoefficentSpectrum& rhs) const{
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = c[i] / rhs.c[i];
            }
            return tmp;
        }

        CoefficentSpectrum operator-() const{
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = -c[i];
            }
            return tmp;
        }
        //assignment ops
        CoefficentSpectrum& operator=(const CoefficentSpectrum& rhs) const{
            for(int i = 0; i < nSamples; ++i){
                c[i] = rhs.c[i];
            }
            return *this;
        }

        CoefficentSpectrum& operator+=(const CoefficentSpectrum& rhs) const{
            for(int i = 0; i < nSamples; ++i){
                c[i] += rhs.c[i];
            }
            return *this;
        }

        CoefficentSpectrum& operator-=(const CoefficentSpectrum& rhs) const{
            for(int i = 0; i < nSamples; ++i){
                c[i] -= rhs.c[i];
            }
            return *this;
        }

        CoefficentSpectrum& operator*=(const CoefficentSpectrum& rhs) const{
            for(int i = 0; i < nSamples; ++i){
                c[i] *= rhs.c[i];
            }
            return *this;
        }

        CoefficentSpectrum& operator/=(const CoefficentSpectrum& rhs) const{
            for(int i = 0; i < nSamples; ++i){
                c[i] /= rhs.c[i];
            }
            return *this;
        }

        friend CoefficentSpectrum Sqrt(const CoefficentSpectrum& rhs) {
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = std::sqrt(rhs.c[i]);
            }
            return tmp;
        }

        friend CoefficentSpectrum Exp(const CoefficentSpectrum& rhs) {
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = std::exp(rhs.c[i]);
            }
            return tmp;
        }

        friend CoefficentSpectrum Pow(const CoefficentSpectrum& rhs, float n) {
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = std::pow(rhs.c[i], n);
            }
            return tmp;
        }

        friend CoefficentSpectrum Lerp(float a, 
                                       const CoefficentSpectrum& s1, 
                                       const CoefficentSpectrum& s2) {
            CoefficentSpectrum tmp;
            for(int i = 0; i < nSamples; ++i){
                tmp.c[i] = (1.f - a) * s1.c[i] + a * s2.c[i];
            }
            return tmp;
        }

   
};

class RGBSpectrum : public CoefficentSpectrum<3> {
    //fromRGB(float r, float g, float b); 
    
    RGBSpectrum(float v = 0.f) : CoefficentSpectrum<3>(v) {}
    RGBSpectrum(const CoefficentSpectrum<3> &v) : CoefficentSpectrum<3>(v) {}
    
    static RGBSpectrum fromRGB(float rgb[3]) {
        RGBSpectrum s;
        s.c[0] = rgb[0];
        s.c[1] = rgb[1];
        s.c[2] = rgb[2];
        return s;
    }

    void toRGB(float rgb[3]) const {
        rgb[0] = c[0]; 
        rgb[1] = c[1]; 
        rgb[2] = c[2]; 
    }

    static RGBSpectrum fromXYZ(float xyz[3]) const {
        RGBSpectrum s;
        s.c[0] = r;
        s.c[1] = g;
        s.c[2] = b;
        return s;
    }

    
    void RGBSpectrum toXYZ(float xyz[3]) const {//TODO this actually not converting
        xyz[0] = c[0]; 
        xyz[1] = c[1]; 
        xyz[2] = c[2]; 
    }
    //static RGBSpectrum fromSampled(); TODO
     

};


#endif
