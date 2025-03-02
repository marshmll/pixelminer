/**
 * @file ToggleButton.hxx
 * @brief Declares the ToggleButton class to handle binary inputs from the user.
 */

#pragma once

#include "GUI/GUI.hxx"
#include "GUI/TextButton.hxx"

namespace gui
{
    /**
     * @class ToggleButton
     * @brief The toggle button is a button that handles binary inputs from the user, i.e., on or off, true or false,
     * etc.
     */
    class ToggleButton : public TextButton
    {
      private:
        std::string toggleMsg;  ///< The primary message of the button. Indicates what the button's value is about.
        sf::Clock toggleClock;  ///< A clock to create a interval between each button toggle.
        uint32_t toggleTimeMax; ///< The max of time between each toggle.
        bool on;                ///< The current value of the button.

      public:
        /**
         * @brief Constructor for the ToggleButton class.
         * @param position The position of the button.
         * @param size The size of the button.
         * @param fill_color The color of the button.
         * @param toggle_msg The message to be shown before the "ON"/"OFF" message. Do not pass "ON" or "OFF" or any
         * other value, as this message is concatenated with ": ON" or ": OFF" strings.
         * @param font A font reference.
         * @param char_size The character size for the text.
         * @param text_color The color of the text (default is white).
         * @param on The value the button is initialized with (default is false).
         * @param toggle_time_max The time between each toggle, in milliseconds (default is 200).
         * @param outline_thickness The outline thickness for the button (default is 0).
         * @param outline_color The outline color for the button (default is transparent).
         */
        ToggleButton(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
                     const std::string toggle_msg, const sf::Font &font, const unsigned int char_size,
                     const sf::Color text_color = sf::Color::White, const bool &on = false,
                     const uint32_t toggle_time_max = 200, const float outline_thickness = 0.f,
                     const sf::Color outline_color = sf::Color::Transparent);

        ~ToggleButton();

        void update(const sf::Vector2f &mouse_pos);

        /**
         * @brief Gets if the button is on or not.
         * @return If the button is on or off.
         */
        const bool &isOn() const;

        /**
         * @brief Sets a new value for the button.
         * @param on The new value.
         */
        void setOn(const bool &on);

        /**
         * @brief Inverts the current value of the button.
         */
        void toggleOn();
    };
} // namespace gui
