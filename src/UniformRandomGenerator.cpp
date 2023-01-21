#include "UniformRandomGenerator.hpp"


UniformRandomGenerator::UniformRandomGenerator()
{
    //rng = std::mt19937(device());
    rng = std::mt19937(rand());
    uniformRand = std::uniform_real_distribution<>(0.f, 1.f);
}

UniformRandomGenerator::UniformRandomGenerator(const UniformRandomGenerator& rhs)
{
    //rng = std::mt19937(device());
    rng = std::mt19937(rand());
    uniformRand = std::uniform_real_distribution<>(0.f, 1.f);
}

UniformRandomGenerator::~UniformRandomGenerator()
{
}

float UniformRandomGenerator::getUniformRandNumber(float min, float max)
{
    float ret;
    lock.lock();//get mutex
    ret = uniformRand(rng);
    lock.unlock();//release mutex
    return ret * (max - min) + min; 
}

