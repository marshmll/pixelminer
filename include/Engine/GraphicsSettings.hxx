#pragma once

#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"

class GraphicsSettings
{
  private:
    Logger logger;

  public:
    unsigned int screenWidth;
    unsigned int screenHeight;
    unsigned int framerateLimit;
    bool fullscreen;
    std::string resourcePack;

    GraphicsSettings();
    ~GraphicsSettings();

    const bool loadFromFile(const std::filesystem::path path);

    const bool saveToFile(const std::filesystem::path path);
};