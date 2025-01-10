#pragma once

#include "Tools/JSON.hxx"

class GraphicsSettings
{
  public:
    unsigned int screenWidth;
    unsigned int screenHeight;
    unsigned int framerateLimit;
    bool fullscreen;
    bool vsync;

    GraphicsSettings();
    ~GraphicsSettings();

    const bool loadFromFile(const std::filesystem::path path);

    const bool saveToFile(const std::filesystem::path path);
};