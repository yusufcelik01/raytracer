#ifndef __THREAD__SAFE__UNIFORM__RANDOM__GENERATOR__
#define __THREAD__SAFE__UNIFORM__RANDOM__GENERATOR__

#include <random> 
#include <mutex> 

class UniformRandomGenerator
{
    private:
        std::random_device device;
        std::mt19937 rng;
        std::uniform_real_distribution<> uniformRand;

        std::mutex lock;

    public:
        UniformRandomGenerator();
        ~UniformRandomGenerator();
        float getUniformRandNumber(float min, float max);
};

#endif
