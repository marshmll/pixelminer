/**
 * @file TerminalColor.hxx
 * @brief Declares the TerminalColor class that manages standard output (stdout/stderr) text colors.
 */

#pragma once

/**
 * @enum TermColor
 * @brief Enum representing terminal colors for text formatting.
 *
 * This enum defines a set of colors that can be used to change the text color in the terminal.
 */
enum TermColor : int
{
    Black = 30,         ///< Black color.
    Red = 31,           ///< Red color.
    Green = 32,         ///< Green color.
    Yellow = 33,        ///< Yellow color.
    Blue = 34,          ///< Blue color.
    Magenta = 35,       ///< Magenta color.
    Cyan = 36,          ///< Cyan color.
    White = 37,         ///< White color.
    BrightBlack = 90,   ///< Bright black (dark gray) color.
    BrightRed = 91,     ///< Bright red color.
    BrightGreen = 92,   ///< Bright green color.
    BrightYellow = 93,  ///< Bright yellow color.
    BrightBlue = 94,    ///< Bright blue color.
    BrightMagenta = 95, ///< Bright magenta color.
    BrightCyan = 96,    ///< Bright cyan color.
    BrightWhite = 97,   ///< Bright white color.
};

/**
 * @class TerminalColor
 * @brief A utility class for handling terminal text color changes.
 *
 * This class provides static methods to set and reset the text color in the terminal
 * using ANSI escape codes. It can be used to format output text in different colors
 * for better visual distinction.
 */
class TerminalColor
{
  public:
    /**
     * @brief Set the terminal text color.
     *
     * This function changes the color of the text output in the terminal based on the
     * specified color code.
     *
     * @param color The color to set. It should be one of the values from the TermColor enum.
     */
    static void set(int color);

    /**
     * @brief Reset the terminal text color to default.
     *
     * This function resets the terminal text color back to the default color, removing
     * any previous color settings.
     */
    static void reset();
};
