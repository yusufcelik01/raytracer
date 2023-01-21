#include "AreaLight.hpp"


AreaLight::~AreaLight()
{
    if(onb != NULL) {
        delete onb;
    }
}

ONB AreaLight::getONB()
{
    if(onb == NULL) {
        onb = new ONB(normal); 
    }
    return *onb;
}

vec3f AreaLight::getLightSample()
{
    ONB onb = getONB();
    vec3f lightSample;
    float xsi1, xsi2;
    //std::random_device rand;
    //std::mt19937 rnGen(rand());
    //std::uniform_real_distribution<> randNum(-0.5*extent, 0.5*extent);

    //psi1 = randNum(rnGen);
    //psi2 = randNum(rnGen);
    xsi1 = rng.getUniformRandNumber(-0.5*extent, 0.5*extent);
    xsi2 = rng.getUniformRandNumber(-0.5*extent, 0.5*extent);

    lightSample = position + onb.u * xsi1 + onb.v * xsi2;

    return lightSample;
}
