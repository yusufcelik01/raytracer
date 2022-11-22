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

