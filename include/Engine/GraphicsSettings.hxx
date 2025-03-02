/**
 * @file GraphicsSettings.hxx
 * @brief Declares the GraphicsSettings class for handling graphical configurations.
 */

#pragma once

#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"

/**
 * @class GraphicsSettings
 * @brief Handles graphical configuration settings.
 */
class GraphicsSettings
{
  private:
    Logger logger; ///< Logger instance for debugging and information logging.

  public:
    unsigned int screenWidth;    ///< Width of the screen resolution.
    unsigned int screenHeight;   ///< Height of the screen resolution.
    unsigned int framerateLimit; ///< Limit for the framerate.
    bool fullscreen;             ///< Flag to determine if fullscreen mode is enabled.
    bool vsync;                  ///< Flag to determine if V-Sync is enabled.
    std::string resourcePack;    ///< Name of the active resource pack.

    /**
     * @brief Constructs a GraphicsSettings instance.
     */
    GraphicsSettings();

    /**
     * @brief Destructor for GraphicsSettings.
     */
    ~GraphicsSettings();

    /**
     * @brief Loads graphical settings from a configuration file.
     * @param path Path to the configuration file.
     * @return True if the settings were successfully loaded, false otherwise.
     */
    const bool loadFromFile(const std::filesystem::path path);

    /**
     * @brief Saves the current graphical settings to a configuration file.
     * @param path Path to the configuration file.
     * @return True if the settings were successfully saved, false otherwise.
     */
    const bool saveToFile(const std::filesystem::path path);
};
