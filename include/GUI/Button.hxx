/**
 * @file Button.hxx
 * @brief Declares the gui::Button class for managing clickable buttons.
 */

#pragma once

#include "GUI/Utils.hxx"

namespace gui
{
    /**
     * @enum ButtonState
     * @brief Enum representing the possible states of a button.
     */
    enum ButtonState
    {
        Idle = 0, ///< Default state of the button.
        Hover,    ///< Button is hovered by the mouse.
        Active,   ///< Button is pressed.
        Disabled, ///< Button is disabled and non-interactive.
    };

    /**
     * @class Button
     * @brief Represents a clickable button in the GUI.
     *
     * Handles button rendering, state management, and interaction with the mouse.
     */
    class Button
    {
      protected:
        sf::RectangleShape body;         ///< The button's body shape.
        sf::RectangleShape brightShadow; ///< The button's bright shadow.
        sf::RectangleShape darkShadow;   ///< The button's dark shadow.

        sf::Color fillColor;    ///< Fill color of the button.
        sf::Color outlineColor; ///< Outline color of the button.
        ButtonState state;      ///< The current state of the button.

      public:
        /**
         * @brief Constructs a Button object.
         *
         * Initializes the button with position, size, color, outline, and other properties.
         *
         * @param position Position of the button.
         * @param size Size of the button.
         * @param fill_color Fill color of the button.
         * @param outline_thickness Thickness of the button outline.
         * @param outline_color Color of the button's outline.
         */
        Button(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
               const float outline_thickness = 0.f, const sf::Color outline_color = sf::Color::Transparent);

        /**
         * @brief Destructor for the Button class.
         *
         * Destroys the Button object.
         */
        virtual ~Button();

        /**
         * @brief Updates the button state.
         *
         * Changes the button state based on mouse position and interaction.
         *
         * @param mouse_pos The current position of the mouse.
         */
        virtual void update(const sf::Vector2f &mouse_pos);

        /**
         * @brief Renders the button on the target.
         *
         * Draws the button's body and shadows on the render target.
         *
         * @param target The render target where the button will be drawn.
         */
        virtual void render(sf::RenderTarget &target);

        /**
         * @brief Checks if the button is pressed.
         *
         * @return True if the button is in the pressed state, false otherwise.
         */
        const bool isPressed() const;

        /**
         * @brief Gets the button's position.
         *
         * @return The position of the button.
         */
        const sf::Vector2f getPosition() const;

        /**
         * @brief Gets the button's size.
         *
         * @return The size of the button.
         */
        const sf::Vector2f getSize() const;

        /**
         * @brief Gets the current button's state.
         *
         * @return The state of the button.
         */
        const ButtonState &getState() const;

        /**
         * @brief Sets the state of the button.
         *
         * @param state The new state for the button.
         */
        void setState(const ButtonState &state);

        /**
         * @brief Sets the position of the button.
         *
         * @param position The new position for the button.
         */
        void setPosition(const sf::Vector2f &position);

        /**
         * @brief Moves the button with a given offset.
         *
         * @param offset The offset to move the button.
         */
        virtual void move(const sf::Vector2f &offset);
    };
} // namespace gui
