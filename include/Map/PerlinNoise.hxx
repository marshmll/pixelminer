#pragma once

#include "Tools/LinearCongruentialGenerator.hxx"

typedef struct
{
    float seed;
    float frequency;
    float amplitude;
} Wave;

using NoiseMap = std::vector<std::vector<float>>;

class PerlinNoise
{
  private:
    static const int PERMUTATION_SIZE = 256;
    std::vector<int> permutation;

    float fade(float t);
    float lerp(float a, float b, float t);
    float grad(int hash, float x, float y);

    float noise(float x, float y);

  public:
    PerlinNoise(const unsigned int &seed);

    ~PerlinNoise();

    const NoiseMap generateNoiseMap(const unsigned int width, const unsigned int height, const float scale,
                                    std::vector<Wave> waves, sf::Vector2f offset);
};