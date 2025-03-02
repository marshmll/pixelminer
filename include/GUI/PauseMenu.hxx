/**
 * @file PauseMenu.hxx
 * @brief Declares the gui::PauseMenu class for handling game pauses.
 */

#pragma once

#include "GUI/Chat.hxx"
#include "GUI/TextButton.hxx"
#include "Map/Map.hxx"
#include "States/State.hxx"
#include "Engine/Languages.hxx"
#include "Network/Server.hxx"

namespace gui
{
    /**
     * @class PauseMenu
     * @brief A class that represents the pause menu in the game.
     *
     * The PauseMenu class provides the functionality for managing the pause
     * menu, which includes buttons for returning to the game, viewing achievements,
     * checking statistics, opening options, and saving or quitting the game.
     *
     * It allows for interactive updates, including button presses, and can be
     * rendered to the screen when the pause menu is active.
     */
    class PauseMenu
    {
      private:
        sf::RectangleShape background;      ///< The background rectangle of the pause menu.
        sf::RectangleShape buttonContainer; ///< The container holding the buttons of the pause menu.
        std::unordered_map<std::string, std::unique_ptr<gui::TextButton>>
            buttons; ///< A map of buttons with their respective keys.

        bool active; ///< Flag to track whether the pause menu is active or not.
        bool quit;   ///< Flag to track whether the user wants to quit.

        EngineData &data; ///< The reference to the engine's data, providing access to resources and data.
        Server &server; ///< The reference to the game's server instance.
        Chat &chat; ///< The referente to the game's chat instance.

      public:
        /**
         * @brief Constructs the PauseMenu with the provided engine data.
         *
         * This constructor initializes the pause menu's background, button container,
         * and individual buttons with their respective properties like position,
         * size, and label.
         *
         * @param data The engine data used to fetch necessary resources and settings.
         */
        PauseMenu(EngineData &data, Server &server, Chat &chat);

        /** Destructor for PauseMenu */
        ~PauseMenu();

        /**
         * @brief Updates the pause menu based on the given delta time and mouse position.
         *
         * This method processes any button interactions, updating button states
         * based on mouse position and checks if any buttons were pressed. It also
         * manages the pause menu's active state.
         *
         * @param dt The delta time since the last update.
         * @param mouse_pos The current position of the mouse.
         */
        void update(const float &dt, const sf::Vector2f &mouse_pos);

        /**
         * @brief Renders the pause menu to the given render target.
         *
         * This method draws the background and buttons of the pause menu onto the
         * specified target (usually the screen).
         *
         * @param target The render target to which the pause menu will be drawn.
         */
        void render(sf::RenderTarget &target);

        /**
         * @brief Checks if the pause menu is currently active.
         *
         * This method returns the current active state of the pause menu.
         *
         * @return A constant reference to the active state flag.
         */
        const bool &isActive() const;

        /**
         * @brief Checks if the user has requested to quit the game.
         *
         * This method returns the quit flag, which indicates whether the user
         * wants to quit the game from the pause menu.
         *
         * @return A constant reference to the quit flag.
         */
        const bool &getQuit() const;

        /**
         * @brief Toggles the active state of the pause menu.
         *
         * This method switches the active state of the pause menu between active and inactive.
         */
        void toggleActive();

        /**
         * @brief Sets the active state of the pause menu.
         *
         * This method explicitly sets the active state of the pause menu.
         *
         * @param active The new active state to set for the pause menu.
         */
        void setActive(const bool &active);

        /**
         * @brief Sets the quit state of the pause menu.
         *
         * This method sets the quit state, indicating whether the user wishes
         * to quit the game.
         *
         * @param quit The new quit state to set for the pause menu.
         */
        void setQuit(const bool &quit);
    };
} // namespace gui
