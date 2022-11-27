#include "UniformRandomGenerator.hpp"


UniformRandomGenerator::UniformRandomGenerator()
{
    rng = std::mt19937(device());
    uniformRand = std::uniform_real_distribution<>(0.f, 1.f);
}

UniformRandomGenerator::~UniformRandomGenerator()
{
    lock.~mutex();
}

float UniformRandomGenerator::getUniformRandNumber(float min, float max)
{
    float ret;
    lock.lock();//get mutex
    ret = uniformRand(rng);
    lock.unlock();//release mutex
    return ret * (max - min) + min; 
}

