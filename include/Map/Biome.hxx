#pragma once

#include "Tiles/TileData.hxx"

enum BiomeType
{
    UnknownBiome = 0,
    Desert,
    Forest,
    Grassland,
    Jungle,
    Mountains,
    Ocean,
    Tundra,
};

struct BiomeData
{
    BiomeType type = BiomeType::UnknownBiome;
    sf::Color color = sf::Color::Transparent;
    std::string baseTileId = "unknown";
};

class Biome
{
  private:
    std::string name;
    BiomeType type;

    float idealHeight;
    float idealMoisture;
    float idealHeat;

    void setBiomeName();

  public:
    Biome(const BiomeType type, const float &ideal_height, const float &ideal_moisture, const float &ideal_heat);
    ~Biome();

    const float calculateWeight(const float &height, const float &moisture, const float &heat) const;

    const BiomeType &getType() const;

    const std::string &getName() const;
};