#pragma once

#include "Map/Biome.hxx"
#include "Map/PerlinNoise.hxx"
#include "Map/TileBase.hxx"

class Map
{
  private:
    sf::Vector3<unsigned int> dimensions;
    std::vector<std::vector<std::vector<std::unique_ptr<TileBase>>>> tiles;

    PerlinNoise noise;

    std::vector<Wave> height_waves;
    std::vector<Wave> moisture_waves;
    std::vector<Wave> heat_waves;

    NoiseMap height_map;
    NoiseMap moisture_map;
    NoiseMap heat_map;

    std::vector<Biome> biomes;

    void clear();

    void resize(const sf::Vector3<unsigned int> &dimensions);

    void initPerlinWaves();

    void initNoiseMaps();

    void initBiomes();

  public:
    Map(const sf::Vector3<unsigned int> dimensions);
    ~Map();

    const sf::Vector3<unsigned int> &getDimensions() const;

    void generate();
};