#pragma once

#include "Tools/JSON.hxx"

class GraphicsSettings
{
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