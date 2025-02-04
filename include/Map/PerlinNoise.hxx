#pragma once

#include "Tools/LinearCongruentialGenerator.hxx"

constexpr int PERMUTATION_SIZE = 256;

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
    std::vector<int> permutation;

    float fade(float t);
    float lerp(float a, float b, float t);
    float grad(int hash, float x, float y);

    float noise(float x, float y);

  public:
    PerlinNoise(const unsigned int &seed);

    ~PerlinNoise();

    const NoiseMap generateNoiseMap(const unsigned int &width, const unsigned int &height, const float &scale,
                                    const std::vector<Wave> &waves, const sf::Vector2f &offset);
};