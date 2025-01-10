#pragma once

class Random
{
  private:
    unsigned long long int current;
    static constexpr unsigned long int a = 1664525;    // Multiplier
    static constexpr unsigned long int c = 1013904223; // Increment
    static constexpr unsigned long int m = 4294967296; // Modulus (2^32)

  public:
    Random(const long long int &seed);

    // Generate a random number in the range [0, 1)
    float nextFloat();

    // Generate a random integer in the range [min, max]
    int nextInt(int min, int max);
};
