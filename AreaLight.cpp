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
    float psi1, psi2;
    std::random_device rand;
    std::mt19937 rnGen(rand());
    std::uniform_real_distribution<> randNum(-0.5*extent, 0.5*extent);

    psi1 = randNum(rnGen);
    psi2 = randNum(rnGen);

    lightSample = position + onb.u * psi1 + onb.v * psi2;

    return lightSample;
}
