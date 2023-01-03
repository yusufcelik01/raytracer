#include "PerlinNoise.hpp"
#include "rtmath.hpp"
#include <cmath>
#include <iostream>

PerlinNoise::PerlinNoise()
{
    noiseScale = 1.f;
    noiseConv = NOISE_CONV_ABSVAL;

}

PerlinNoise::PerlinNoise(float scale, const std::string& conversion)
{
    noiseScale = scale;
    if(conversion == "linear")
    {
        noiseConv = NOISE_CONV_LINEAR;
    }
    //else if(conversion == "absval")
    else 
    {
        noiseConv = NOISE_CONV_ABSVAL;
    }

}


vec3f gradients[16] = {
    vec3f(1, 1, 0),
    vec3f(-1, 1, 0),
    vec3f(1, -1, 0),
    vec3f(-1, -1, 0),
    vec3f(1, 0, 1),
    vec3f(-1, 0, 1),
    vec3f(1, 0, -1),
    vec3f(-1, 0, -1),
    vec3f(0, 1, 1),
    vec3f(0, -1, 1),
    vec3f(0, 1, -1),
    vec3f(0, -1, -1),
    vec3f(1, 1, 0),
    vec3f(-1, 1, 0),
    vec3f(0, -1, 1),
    vec3f(0, -1, -1)
};

int table[16] = {
    14, 8, 9, 7, 5, 13, 4, 0, 12, 2, 3, 11, 6, 15, 10, 1
};

float fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

vec3f grad(int i, int j, int k)
{
    if(i < 0) i = -i;
    if(j < 0) j = -j;
    if(k < 0) k = -k;
    int ind;
    ind = table[i % 16];
    ind = table[(j + ind) % 16];
    ind = table[(k + ind) % 16];
    
    return gradients[ind];
}
//float perlinNoise(vec3f texCoords)

vec3f PerlinNoise::sample(vec3f texCoord)
{
    //std::cout << "perlin noise sample function" << std::endl;
    if(colorMap.size() < 1)
    {
        float x, y;
        x = texCoord.x;
        y = texCoord.y;
        float r = sampleScalar(vec3f(x, y, 0.f));
        float g = sampleScalar(vec3f(0.f, x, y));
        float b = sampleScalar(vec3f(x, 0.f, y));
        return vec3f(r, g, b);
    }
    float s = sampleScalar(texCoord);
    int index = int(s * (colorMap.size() -1));
    //std::cout << "perlin noise color map index: " << index << std::endl;
    return colorMap[index];
}

vec3f PerlinNoise::sample(float x, float y)
{
    //return sample(vec3f(x, y, (x+y)/2));
    float r = sampleScalar(vec3f(x, y, 0.f));
    float g = sampleScalar(vec3f(0.f, x, y));
    float b = sampleScalar(vec3f(x, 0.f, y));
    return vec3f(r, g, b);
}

float PerlinNoise::sampleScalar(vec3f texCoords)
{
    //using glm::mix, glm::dot, glm::floor;
    texCoords = texCoords * noiseScale;
    //texCoords = float(pow(2, -1)) * texCoords;

    int i = int(floor(texCoords.x)) & 255,
        j = int(floor(texCoords.y)) & 255,
        k = int(floor(texCoords.z)) & 255;

    float x = texCoords.x - floor(texCoords.x),
          y = texCoords.y - floor(texCoords.y),
          z = texCoords.z - floor(texCoords.z);
    
    float u = fade(x),
          v = fade(y),
          w = fade(z);

    vec3f g000 = grad(i  , j  , k  ),
          g100 = grad(i+1, j  , k  ),
          g110 = grad(i+1, j+1, k  ),
          g010 = grad(i  , j+1, k  ),
          g011 = grad(i  , j+1, k+1),
          g111 = grad(i+1, j+1, k+1),
          g101 = grad(i+1, j  , k+1),
          g001 = grad(i  , j  , k+1);

    vec3f p000 = vec3f(x  , y  , z  ),
          p100 = vec3f(x-1, y  , z  ), 
          p110 = vec3f(x-1, y-1, z  ), 
          p010 = vec3f(x  , y-1, z  ),
          p011 = vec3f(x  , y-1, z-1), 
          p111 = vec3f(x-1, y-1, z-1), 
          p101 = vec3f(x-1, y  , z-1),
          p001 = vec3f(x  , y  , z-1); 


    float c; 
    c = mix(mix(mix(dot(g000, p000), dot(g100, p100), u),
                mix(dot(g010, p010), dot(g110, p110), u), v),

            mix(mix(dot(g001, p001), dot(g101, p101), u),
                mix(dot(g011, p011), dot(g111, p111), u), v), w);



    if(noiseConv == NOISE_CONV_LINEAR)
    {
        return (c+1)/2.0f;
    }
    else//absval
    {
        return (c <= 0)? -c : c;
    }
}


vec3f PerlinNoise::sampleDu(float x, float y)
{ 
    return sample(x+ TEX_SAMPLING_EPSILON*100.f,y);
}

vec3f PerlinNoise::sampleDv(float x, float y)
{
    return sample(x,y + TEX_SAMPLING_EPSILON*100.f);
}
