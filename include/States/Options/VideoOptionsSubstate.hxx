/**
 * @file VideoOptionsSubstate.hxx
 * @brief Header file for the VideoOptionsSubstate class.
 */

#pragma once

#include "States/Substate.hxx"
#include "GUI/GUI.hxx"

/**
 * @class VideoOptionsSubstate
 * @brief Represents the video options substate in the application.
 *
 * This class is responsible for managing the video settings menu, including rendering the background,
 * buttons, and sliders for adjusting resolution, framerate limit, fullscreen mode, and VSync. It also
 * handles user interactions with the buttons and sliders, applying the selected video settings.
 */
class VideoOptionsSubstate : public Substate
{
  private:
    Logger logger; ///< Logger object for logging messages.

    sf::RectangleShape background; ///< Background shape for the video options menu.

    sf::RectangleShape buttonContainer; ///< Container for the buttons in the video options menu.
    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons; ///< Map of buttons in the video options menu.
    std::unordered_map<std::string, std::unique_ptr<gui::SliderInput>> sliderInputs; ///< Map of slider inputs for video settings.

    std::string currentResolution; ///< Current resolution setting.
    int currentResolutionIndex; ///< Index of the current resolution in the resolutions list.

    unsigned int minFramerate; ///< Minimum allowed framerate.
    unsigned int maxFramerate; ///< Maximum allowed framerate.

    std::vector<std::string> resolutions; ///< List of available screen resolutions.

    /**
     * @brief Initializes the variables for the video options menu.
     *
     * This function sets up the current resolution, framerate limits, and populates the list of available resolutions.
     */
    void initVariables();

    /**
     * @brief Initializes the GUI elements for the video options menu.
     *
     * This function sets up the background and initializes the buttons and sliders.
     */
    void initGUI();

    /**
     * @brief Initializes the buttons for the video options menu.
     *
     * This function creates and positions the buttons for toggling fullscreen mode, VSync, and applying settings.
     */
    void initButtons();

    /**
     * @brief Applies the selected video settings.
     *
     * This function updates the video settings based on user input and applies them to the game window.
     */
    void applyVideoSettings();

    /**
     * @brief Saves the current graphics settings to a file.
     *
     * @return True if the settings were successfully saved, false otherwise.
     */
    const bool saveGraphicsSettings();

  public:
    /**
     * @brief Constructs a VideoOptionsSubstate object.
     *
     * @param data Reference to the EngineData object containing necessary resources and settings.
     */
    VideoOptionsSubstate(EngineData &data);

    /**
     * @brief Destructor for the VideoOptionsSubstate object.
     */
    ~VideoOptionsSubstate();

    /**
     * @brief Updates the state of the video options menu.
     *
     * This function handles user interactions with the buttons and sliders and updates the state accordingly.
     *
     * @param dt The delta time since the last update.
     */
    void update(const float &dt);

    /**
     * @brief Renders the video options menu.
     *
     * This function draws the background, buttons, and sliders to the render target.
     *
     * @param target The render target to draw the video options menu on.
     */
    void render(sf::RenderTarget &target);
};
