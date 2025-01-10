#include "Tools/LinearCongruentialGenerator.hxx"
#include "stdafx.hxx"

Random::Random(const long long int &seed)
{
    unsigned long long int tmp;
    std::memcpy(&tmp, &seed, sizeof(tmp)); // Interpret signed as unsigned
    current = tmp;
}

// Generate a random number in the range [0, 1)
float Random::nextFloat()
{
    current = (a * current + c) % m;
    return static_cast<float>(current) / m;
}

// Generate a random integer in the range [min, max]
int Random::nextInt(int min, int max)
{
    return min + static_cast<int>(nextFloat() * (max - min + 1));
}