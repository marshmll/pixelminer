/**
 * @file Input.hxx
 * @brief Declares the Input class for gathering textual input from the user's keyboard.
 */

#pragma once

#include "GUI/GUI.hxx"

/**
 * @brief Mapping of SFML keyboard keys to character values.
 *
 * This unordered map defines how the SFML keyboard keys are mapped to their
 * corresponding character values (e.g., 'A' is mapped to 'a', '1' to '1').
 * Special keys and symbols are handled as well, like Shift-modified characters.
 */
static std::unordered_map<sf::Keyboard::Key, char> keyMap;

/**
 * @brief List of special keys that require custom handling.
 *
 * This list contains keys like Escape, Enter, Space, and Arrow keys that
 * are treated differently from regular alphanumeric keys.
 */
static std::vector<sf::Keyboard::Key> specialKeys;

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

    sf::Clock keyTimer;               ///< Timer used to detect key repetition.
    sf::Keyboard::Key lastKeyPressed; ///< The last key pressed to handle key repetition.
    bool repeat;                      ///< Flag indicating whether a key is being held down for repetition.

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
     * @brief Handles the tab key press.
     *
     * This method inserts spaces to simulate the tab behavior in the input field.
     */
    void handleTab();

    /**
     * @brief Handles the enter key press.
     *
     * This method inserts a newline character at the cursor position when
     * the enter key is pressed.
     */
    void handleEnter();

    /**
     * @brief Handles arrow key presses (left/right).
     *
     * This method moves the cursor left or right based on the arrow key pressed,
     * preventing it from going out of bounds.
     *
     * @param key The arrow key pressed (left or right).
     */
    void handleArrowKey(sf::Keyboard::Key key);

    /**
     * @brief Handles special keys like Escape and Space.
     *
     * Special keys such as Escape, Space, and arrow keys are handled by this
     * method, performing actions like inserting spaces or exiting input modes.
     *
     * @param key The special key pressed.
     */
    void handleSpecialKey(sf::Keyboard::Key key);

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
     */
    Input(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &body_color, sf::Font &font,
          const unsigned int &char_size, const float &padding, const float &outline_thickness,
          const sf::Color outline_color, const std::string label = "");

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
     * @brief Clears the text from the input field.
     *
     * This method removes all the entered text and resets the cursor position.
     */
    void clear();
};
} // namespace gui
