#include "tonemap.hpp"
#include <cmath>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

namespace tonemap
{
    const double delta = 0.00001;    
unsigned char* photographicToneMap(float* hdrData, int w, int h,int comp ,float keyValue, float burnPercent, float saturation, float gamma)
{
    assert(comp == 3 || comp == 4);//only accept RGB or RGBA
    float midGray = 0.f;//log-average-luminance
    std::vector<float> sortedLum = {};
    float Lmin =  std::numeric_limits<float>::max();
    float Lmax = -std::numeric_limits<float>::max();

    float* inLum = new float[w*h];
    float* outLum = new float[w*h];
    float* tempRGB = new float[w*h*3];//width*height*RGB
    unsigned char* outRGB = new unsigned char[w*h*comp];
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            float r,g,b;
            r = hdrData[(y*w + x)*comp + 0];
            g = hdrData[(y*w + x)*comp + 1];
            b = hdrData[(y*w + x)*comp + 2];
            float L =  0.2126*r + 0.7152*g + 0.0722*b;
            inLum[(y*w + x)] = L;
            outLum[(y*w + x)] = L;
            Lmin = (L < Lmin) ? L : Lmin;//get Lmin
            Lmax = (L > Lmax) ? L : Lmax;//get Lmax
            float temp = log(delta + L);
            if(temp != temp)
            {
                std::cout <<"NaN value present quitting" << std::endl;
                exit(-1);
            }
            midGray += temp;
        }
    }
    std::cout << "Lmin: " << Lmin << std::endl;
    std::cout << "Lmax: " << Lmax << std::endl;
    midGray = exp(midGray/(w*h));
    //midGray = exp(midGray)/w*h;
    std::cout << "mid-gray: " << midGray << std::endl;
    float scaleFactor = keyValue / midGray;


    //apply linear scaling 
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            outLum[y*w + x] *= scaleFactor;
            sortedLum.push_back(outLum[y*w + x]);
        }
    }


    //float Lwhite = 0.f;
    std::sort(sortedLum.begin(), sortedLum.end(), std::greater<float>());
    float Lwhite = sortedLum[int((sortedLum.size() * (burnPercent)/100.f) + 0.5f)];
    //Lwhite = Lmax;
    std::cout << "Lwhite: " << Lwhite << std::endl;
    
    //apply sigmoidal transform
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            float L = outLum[y*w + x];
            outLum[y*w + x] = (L * (1.f + L/(Lwhite*Lwhite))) / (1.f + L);
        } 
    }
    
    //compute new RGB
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            float Lout = outLum[y*w + x];
            float Lin  = inLum[y*w + x];
            float inR, inG, inB;
            inR = hdrData[(y*w + x)*comp + 0];
            inG = hdrData[(y*w + x)*comp + 1];
            inB = hdrData[(y*w + x)*comp + 2];
            
            tempRGB[(y*w + x)*3 + 0] = std::clamp(Lout * pow(inR/Lin, saturation), 0., 1.);
            tempRGB[(y*w + x)*3 + 1] = std::clamp(Lout * pow(inG/Lin, saturation), 0., 1.);
            tempRGB[(y*w + x)*3 + 2] = std::clamp(Lout * pow(inB/Lin, saturation), 0., 1.);
        } 
    }

    //copy alpha values if present
    if(comp == 4)
    {
        for(int y = 0; y < h; ++y)
        {
            for(int x = 0; x < w; ++x)
            {
                outRGB[(y*w + x)*4 + 3] = int(255 *hdrData[(y*w + x)*4 + 3]);
            } 
        }
    }
   
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            outRGB[(y*w + x)*comp + 0] = int(255 *pow(tempRGB[(y*w + x)* 3 + 0], 1.f/gamma));
            outRGB[(y*w + x)*comp + 1] = int(255 *pow(tempRGB[(y*w + x)* 3 + 1], 1.f/gamma));
            outRGB[(y*w + x)*comp + 2] = int(255 *pow(tempRGB[(y*w + x)* 3 + 2], 1.f/gamma));
        } 
    }

    
    delete[] inLum;
    delete[] outLum;
    delete[] tempRGB;

    return outRGB;
}

}//namespace tonemap end
