/**
 * @file Utils.hxx
 * @brief Declares Utils functions to help calculating ideal sizes for given screen resolutions.
 */

#pragma once

namespace gui
{
    /**
     * @brief Calculates a percentage of a given value.
     *
     * This function computes the value that corresponds to a specified percentage of the input value.
     * For example, calling `percent(100, 20)` would return `20` as 20% of 100.
     *
     * @param value The base value to calculate the percentage of.
     * @param percent The percentage to apply (0-100).
     * @return The calculated percentage of the input value.
     */
    static float percent(float value, float percent)
    {
        return value * (percent / 100.f);
    }

    /**
     * @brief Calculates a character size based on the video mode dimensions.
     *
     * This function calculates an appropriate character size for text rendering based on the resolution
     * of the screen (video mode). It uses the combined width and height of the screen and divides by a given
     * divisor (default is 60) to scale the character size accordingly.
     *
     * @param vm The current video mode (screen resolution).
     * @param divisor The divisor to scale the character size. Default value is 60.
     * @return The calculated character size based on the screen resolution.
     */
    static unsigned int charSize(sf::VideoMode &vm, unsigned int divisor = 60)
    {
        return static_cast<unsigned int>((vm.size.x + vm.size.y) / divisor);
    }
} // namespace gui
