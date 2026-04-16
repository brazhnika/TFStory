#pragma once
#include <random>

static int MaxVal = 200;

class RandomGenerator
{
public:
    static RandomGenerator& Get();
    int GetInt();
    float GetProbability()
    {
        return static_cast<float>(RandomGenerator::Get().GetInt())/static_cast<float>(MaxVal);
    }
private:
    RandomGenerator();

    std::random_device m_dev;
    std::default_random_engine m_engine;
    std::uniform_int_distribution<int> m_distr;
};