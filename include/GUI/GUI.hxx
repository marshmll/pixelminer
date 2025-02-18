/**
 * @file
 * @brief Includes the necessary GUI components for the game's interface.
 *
 * This file includes all the necessary GUI components required for building the user interface
 * of the game. These components handle user interaction elements such as buttons, input fields,
 * menus, scrollable containers, and utility functions.
 *
 * Each component is designed to be used for managing and handling user input in the game,
 * enabling the game's menus and interface to function properly.
 */

#pragma once

/**
 * @namespace GUI
 * @brief The GUI namespace aggregates common user interface artifacts such as lists, buttons, menus
 * and similar others.
 *
 * This namespace implements the UI artifacts only with resources provided by SFML.
 */

/**
 * @brief Includes the Button class for handling clickable buttons in the GUI.
 *
 * This header defines the `Button` class, which represents a clickable button in the GUI. The
 * `Button` class is responsible for managing button interactions, such as mouse clicks and events.
 * It provides functionality to visually represent the button and trigger actions when clicked.
 */
#include "GUI/Button.hxx"

/**
 * @brief Includes the Input class for handling user input fields.
 *
 * This header defines the `Input` class, which represents text input fields in the GUI. The
 * `Input` class allows users to type and submit text in the interface, such as entering names
 * or other data.
 */
#include "GUI/Input.hxx"

/**
 * @brief Includes the PauseMenu class for handling the pause menu in the game.
 *
 * This header defines the `PauseMenu` class, which is responsible for displaying and managing
 * the pause menu in the game. It allows the player to pause the game and interact with different
 * options, such as resuming the game or quitting.
 */
#include "GUI/PauseMenu.hxx"

/**
 * @brief Includes the ScrollableContainer class for handling scrollable content in the GUI.
 *
 * This header defines the `ScrollableContainer` class, which represents a container with scrollable
 * content in the GUI. It enables the display of large amounts of data, such as lists or inventories,
 * within a fixed space by providing scroll functionality.
 */
#include "GUI/ScrollableContainer.hxx"

/**
 * @brief Includes the TextButton class for handling buttons with text in the GUI.
 *
 * This header defines the `TextButton` class, which represents a button with a text label. The
 * `TextButton` class allows users to interact with buttons that display text, enabling actions to
 * be performed when the button is clicked.
 */
#include "GUI/TextButton.hxx"

/**
 * @brief Includes the ImageButton class for handling buttons with text in the GUI.
 *
 * This header defines the `ImageButton` class, which represents a button with a text label. The
 * `ImageButton` class allows users to interact with buttons that display a imgae, enabling actions to
 * be performed when the button is clicked.
 */
#include "GUI/ImageButton.hxx"

/**
 * @brief Includes utility functions for GUI-related tasks.
 *
 * This header provides various utility functions for managing and manipulating GUI components.
 * These functions assist with tasks such as event handling, layout management, and rendering.
 */
#include "GUI/Utils.hxx"
