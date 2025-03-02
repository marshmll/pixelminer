/**
 * @file SliderInput.hxx
 * @brief Declares the SliderInput class for mananing range-based user inputs.
 */

#pragma once

#include "GUI/Button.hxx"
#include "GUI/Utils.hxx"
#include "Tools/Assert.hxx"

namespace gui
{
    /**
     * @class SliderInput
     * @brief The SliderInput class allows the user to slide and select a option based on a range of values.
     * @note The SliderInput class handles only numeric values. Associating them to a different type needs to be
     * done manually.
     */
    class SliderInput
    {
      private:
        sf::RectangleShape body;              ///< The body of the input.
        std::unique_ptr<gui::Button> slider;  ///< A button representing the slider.
        std::unique_ptr<sf::Text> text;       ///< The input's text.
        std::unique_ptr<sf::Text> textShadow; ///< The input's text shadow.

        int selectedOption;     ///< The selected option as a number of the range `0 <= n < ma`x
        unsigned int maxOption; ///< The max option to select such as `n < max`.

      public:
        /**
         * @class SliderInput
         * @brief Constructor for the SliderInput class.
         * @param size The size of the input.
         * @param position The position of the input.
         * @param body_color The color of the input.
         * @param outline_color The outline color of the input.
         * @param outline_thickness The outline thickness of the input.
         * @param slider_thickness The width (thickness) of the slider.
         * @param slider_color The color of the slider.
         * @param slider_outline_color The outline color of the slider.
         * @param slider_outline_thickness The outline thickness of the slider.
         * @param font A font reference.
         * @param str The string content of the input.
         * @param char_size The character size of the text.
         * @param max_option The max option to be selected such as n < max.
         * @param selected_option The initial selected option, note that `selected_option` needs to be less than
         * `max_option`.
         */
        SliderInput(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &body_color,
                    const sf::Color &outline_color, const float &outline_thickness, const float &slider_thickness,
                    const sf::Color &slider_color, const sf::Color slider_outline_color,
                    const float &slider_outline_thickness, sf::Font &font, const std::string &str,
                    const unsigned int &char_size, const unsigned int &max_option,
                    const unsigned int selected_option = 0);

        /**
         * @brief Destructor for the SliderInput class.
         */
        ~SliderInput();

        /**
         * @brief Updates the slider input based on the mouse input.
         */
        void update(const sf::Vector2f &mouse_pos);

        /**
         * @brief Renders the input into a target.
         */
        void render(sf::RenderTarget &target);

        /**
         * @brief Gets what options is currently selected in a range of `0 <= n < max`.
         */
        const int &getSelectedOption() const;

        /**
         * @brief Gets the max option such as `n < max`.
         */
        const unsigned int &getMaxOption() const;

        /**
         * @brief Sets the input's string.
         */
        void setString(const std::string &str);
    };
} // namespace gui
