/**
 * @file Input.hxx
 * @brief Declares the gui::Input class for gathering textual input from the user's keyboard.
 */

#pragma once

#include "GUI/Utils.hxx"

namespace gui
{
/**
 * @class Input
 * @brief A class that represents a text input field in the GUI.
 *
 * This class provides functionality for handling user input within a designated
 * rectangular area. It supports basic text input, cursor visibility, special
 * keys (such as backspace, tab, and enter), and visual rendering.
 *
 * The input field features a blinking cursor and can be cleared or updated
 * based on user actions and event processing.
 */
class Input
{
  private:
    sf::RectangleShape body;         ///< The background rectangle shape of the input field.
    std::unique_ptr<sf::Text> label; ///< The label text displayed above the input field.
    std::unique_ptr<sf::Text> text;  ///< The text entered by the user within the input field.

    unsigned int charSize; ///< The font size of the text.
    float padding;         ///< Padding around the text within the input field.

    sf::RectangleShape blinkerCursor; ///< The cursor that blinks in the input field.
    float cursorTimer;                ///< Timer used to control cursor visibility.
    float cursorTimerMax;             ///< The maximum time between cursor visibility toggles.
    int cursorIndex;                  ///< The current position of the cursor in the text.

    sf::Clock keyTimer;      ///< Timer used to detect key repetition.
    char32_t lastKeyPressed; ///< The last char32_t read from the keyboard.
    bool repeat;             ///< Flag indicating whether a key is being held down for repetition.

    bool focus; ///< Flag indicating whether the input is on focus and keyboard input should be colleted.

    /**
     * @brief Toggles the visibility of the blinking cursor.
     *
     * This method switches the visibility of the cursor between visible and
     * transparent based on the cursor timer.
     */
    void toggleCursorVisibility();

    /**
     * @brief Handles the press of a regular key.
     *
     * This method processes the input of a character, checking for Shift key
     * combinations (for example, '1' becomes '!', '2' becomes '@').
     *
     * @param c The character to be inserted into the text field.
     */
    void handleKeyPress(char32_t c);

    /**
     * @brief Handles the backspace key press.
     *
     * This method removes the character before the cursor if the backspace key
     * is pressed, and adjusts the cursor index accordingly.
     */
    void handleBackspace();

    /**
     * @brief Handles arrow key presses (left/right).
     *
     * This method moves the cursor left or right based on the arrow key pressed,
     * preventing it from going out of bounds.
     *
     * @param key The arrow key pressed (left or right).
     */
    void handleArrowKey(sf::Keyboard::Key key);

  public:
    /**
     * @brief Constructs the Input object with the given parameters.
     *
     * This constructor initializes the input field with its position, size, color,
     * font, and other properties, including the label above the field.
     *
     * @param position The position of the input field.
     * @param size The size of the input field.
     * @param body_color The background color of the input field.
     * @param font The font to be used for the text.
     * @param char_size The character size for the input text.
     * @param padding Padding inside the input field.
     * @param outline_thickness The thickness of the input field's border.
     * @param outline_color The color of the border around the input field.
     * @param label The label displayed above the input field (optional).
     * @param focus If the input should start already on focus (default is false).
     */
    Input(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &body_color, sf::Font &font,
          const unsigned int &char_size, const float &padding, const float &outline_thickness,
          const sf::Color outline_color, const std::string label = "", const bool &focus = false);

    /** Destructor for Input */
    ~Input();

    /**
     * @brief Updates the input field based on the passed delta time and events.
     *
     * This method checks for keyboard input, updates the cursor visibility,
     * and adjusts the position of the cursor based on user actions. It also
     * handles special keys like Backspace, Tab, and Enter.
     *
     * @param dt The delta time since the last update.
     * @param mouse_pos The position of the mouse on the screen.
     * @param event An optional event that might trigger changes in the input.
     */
    void update(const float &dt, sf::Vector2f mouse_pos, std::optional<sf::Event> &event);

    /**
     * @brief Renders the input field to the given render target.
     *
     * This method draws the input field's background, label, text, and the blinking
     * cursor to the provided render target.
     *
     * @param target The render target to which the input field will be drawn.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Returns the current text in the input field as a string.
     *
     * @return The current string entered by the user.
     */
    const std::string getValue() const;

    /**
     * @brief Returns if the input is on focus and should be updated.
     *
     * @return True if the input is on focus, false otherwise.
     */
    const bool &getFocus() const;

    /**
     * @brief Sets a new value for the focus flag.
     *
     * The focus flag tells if the input should be updated.
     *
     * @param focus The new value for focus flag.
     */
    void setFocus(const bool &focus);

    /**
     * @brief Clears the text from the input field.
     *
     * This method removes all the entered text and resets the cursor position.
     */
    void clear();
};
} // namespace gui
