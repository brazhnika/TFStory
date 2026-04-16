#include "RandomGenerator.h"



RandomGenerator& RandomGenerator::Get()
{
    static RandomGenerator generator;
    return generator;
}

RandomGenerator::RandomGenerator()
   : m_dev()
   , m_engine(m_dev())
   , m_distr(0, MaxVal)
{
}

int RandomGenerator::GetInt()
{
   return m_distr(m_engine);
}

