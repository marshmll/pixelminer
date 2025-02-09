/**
 * @file PerlinNoise.hxx
 * @brief Declares the PerlinNoise class that implements the perlin noise algorith to generate noise maps.
 */

#pragma once

#include "Tools/LinearCongruentialGenerator.hxx"

/**
 * @brief Standard size of the permutation vector size for the perlin noise algorithm.
 */
constexpr int PERMUTATION_SIZE = 256;

/**
 * @struct Wave
 * @brief Represents a wave with seed, frequency, and amplitude properties.
 */
struct Wave
{
    float seed;      ///< The seed for the wave.
    float frequency; ///< The frequency of the wave.
    float amplitude; ///< The amplitude of the wave.
};

/**
 * @typedef NoiseMap
 * @brief A map representing the noise values in a 2D grid.
 */
using NoiseMap = std::vector<std::vector<float>>;

/**
 * @class PerlinNoise
 * @brief A class for generating Perlin noise with multiple waves.
 *
 * This class generates 2D Perlin noise, which can be used for various applications like terrain generation
 * or procedural textures. It supports adding multiple waves with different frequencies and amplitudes to create
 * complex noise patterns.
 */
class PerlinNoise
{
  private:
    std::vector<int> permutation; ///< The permutation vector used for the noise generation.

    /**
     * @brief Fade function to smooth the transitions between noise values.
     *
     * The fade function smooths the interpolation between grid points.
     * It is commonly used in Perlin noise generation to ease the transitions.
     *
     * @param t The value to fade, typically between 0 and 1.
     * @return The faded value.
     */
    float fade(float t);

    /**
     * @brief Linear interpolation between two values.
     *
     * This function performs linear interpolation between two values based on a parameter `t`.
     *
     * @param a The first value.
     * @param b The second value.
     * @param t The interpolation parameter, typically between 0 and 1.
     * @return The interpolated value.
     */
    float lerp(float a, float b, float t);

    /**
     * @brief Gradient function to calculate the gradient based on a hash value.
     *
     * The gradient is used to perturb the noise values based on the hash value. It determines how the noise
     * value changes in a given direction.
     *
     * @param hash The hash value used to compute the gradient.
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     * @return The gradient value.
     */
    float grad(int hash, float x, float y);

    /**
     * @brief Generate Perlin noise at a specific point.
     *
     * This function generates the Perlin noise value at a given point (x, y) based on the permutation vector.
     *
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     * @return The noise value at the point, normalized to [0, 1].
     */
    float noise(float x, float y);

  public:
    /**
     * @brief Constructs a PerlinNoise object with a given seed.
     *
     * The constructor initializes the permutation vector and prepares the object for generating Perlin noise.
     *
     * @param seed The seed for the random number generator used in the permutation.
     */
    PerlinNoise(const unsigned int &seed);

    /**
     * @brief Destructor for the PerlinNoise class.
     *
     * The destructor does not perform any specific actions in this class, as the class handles memory management
     * through standard containers.
     */
    ~PerlinNoise();

    /**
     * @brief Generates a 2D noise map based on multiple waves.
     *
     * This function generates a 2D noise map using Perlin noise, with support for combining multiple waves. Each
     * wave can have its own frequency, amplitude, and seed. The final noise value at each point is the sum of
     * the noise from each wave, normalized by their amplitudes.
     *
     * @param width The width of the noise map.
     * @param height The height of the noise map.
     * @param scale The scale of the noise, affecting the spacing between noise features.
     * @param waves A vector of waves to combine in the noise generation process.
     * @param offset The offset to apply to the noise map to shift the generated noise pattern.
     * @return A 2D NoiseMap representing the generated noise.
     */
    const NoiseMap generateNoiseMap(const unsigned int &width, const unsigned int &height, const float &scale,
                                    const std::vector<Wave> &waves, const sf::Vector2f &offset);
};
