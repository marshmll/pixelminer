/**
 * @file TextButton.hxx
 * @brief Declares the gui::TextButton class to manage GUI clicklable buttons with textual content.
 */

#pragma once

#include "GUI/Button.hxx"

namespace gui
{
/**
 * @class TextButton
 * @brief A button that displays text with a shadow effect.
 *
 * The TextButton class is a type of button that displays a string of text with a shadow effect. It inherits from
 * the Button class and allows for interaction with the mouse. The button can change its appearance when hovered or
 * clicked. Additionally, it allows customization of the text color, font, and other visual properties.
 */
class TextButton : public Button
{
  private:
    std::unique_ptr<sf::Text> text;       ///< The text displayed on the button.
    std::unique_ptr<sf::Text> textShadow; ///< The shadow effect of the text.
    sf::Color textColor;                  ///< The color of the text.

  public:
    /**
     * @brief Constructs a TextButton with the specified properties.
     *
     * This constructor initializes a TextButton with a given position, size, fill color, text, font, character size,
     * and text color. Optionally, the button's outline thickness and outline color can also be specified.
     *
     * @param position The position of the button on the screen.
     * @param size The size of the button.
     * @param fill_color The fill color of the button.
     * @param str The string of text displayed on the button.
     * @param font The font used to render the text.
     * @param char_size The size of the characters in the text.
     * @param text_color The color of the text (default is white).
     * @param outline_thickness The thickness of the button's outline (default is 0).
     * @param outline_color The color of the button's outline (default is transparent).
     */
    TextButton(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color, const std::string str,
               const sf::Font &font, const unsigned int char_size, const sf::Color text_color = sf::Color::White,
               const float outline_thickness = 0.f, const sf::Color outline_color = sf::Color::Transparent);

    /** Destructor for TextButton */
    ~TextButton();

    /**
     * @brief Updates the state of the button based on the mouse position.
     *
     * This method checks the mouse position to determine whether the button is hovered or clicked.
     * It updates the button's state (Idle, Hover, Active, Disabled) and adjusts the button's appearance accordingly.
     *
     * @param mouse_pos The current position of the mouse.
     */
    void update(const sf::Vector2f &mouse_pos);

    /**
     * @brief Renders the button to the specified render target.
     *
     * This method draws the button, including the text and shadow, onto the given render target.
     * It adjusts the rendering based on the button's current state.
     *
     * @param target The render target to draw the button on.
     */
    void render(sf::RenderTarget &target);
};
} // namespace gui
