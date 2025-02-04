#pragma once

#include "Engine/Configuration.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"
#include "Tools/Zip.hxx"

class ResourcePack
{
  private:
    Logger logger;

  public:
    ResourcePack();

    std::string name;
    std::string description;
    sf::Texture icon;

    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, TileData> tileDB;

    const bool load(const std::string &name);

    sf::Texture &getTexture(const std::string &key);
};