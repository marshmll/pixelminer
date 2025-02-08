/**
 * @file LinearCongruentialGenerator.hxx
 * @brief Declares the LinearCongruentialGenerator class to create a deterministic Random Number Generator (RNG).
 */

#pragma once

/**
 * @class Random
 * @brief A random number generator class using the Linear Congruential Generator (LCG) algorithm.
 *
 * This class generates pseudo-random numbers based on a linear congruential generator (LCG) algorithm.
 * It supports generating random floating-point numbers in the range [0, 1) and integer numbers within a specified range
 * [min, max].
 */
class Random
{
  private:
    unsigned long long int current;                    ///< The current state of the generator.
    static constexpr unsigned long int a = 1664525;    ///< Multiplier for the LCG.
    static constexpr unsigned long int c = 1013904223; ///< Increment for the LCG.
    static constexpr unsigned long int m = 4294967296; ///< Modulus for the LCG (2^32).

  public:
    /**
     * @brief Constructor that initializes the random number generator with a given seed.
     *
     * The seed is used to set the initial state of the generator. It is treated as an unsigned long long integer
     * by copying its value from the provided signed long long integer.
     *
     * @param seed The initial seed for the random number generator (should be a signed long long integer).
     */
    Random(const long long int &seed);

    /**
     * @brief Generates a random floating-point number in the range [0, 1).
     *
     * The number generated is a pseudo-random value based on the internal state of the generator.
     *
     * @return A random floating-point number in the range [0, 1).
     */
    float nextFloat();

    /**
     * @brief Generates a random integer in the range [min, max].
     *
     * This function generates a pseudo-random integer value within the specified range [min, max],
     * using the `nextFloat()` function to generate the required random floating-point values.
     *
     * @param min The minimum integer value (inclusive).
     * @param max The maximum integer value (inclusive).
     *
     * @return A random integer value in the range [min, max].
     */
    int nextInt(int min, int max);
};
