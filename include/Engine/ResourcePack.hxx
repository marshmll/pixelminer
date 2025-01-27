#pragma once

#include "Engine/Configuration.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"

class ResourcePack
{
  private:
    Logger logger;

  public:
    ResourcePack();

    std::string name;
    std::string description;
    sf::Texture icon;

    std::map<std::string, sf::Font> fonts;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, TileData> tileDB;

    void load(const std::string &name);

    sf::Texture &getTexture(const std::string &key);
};