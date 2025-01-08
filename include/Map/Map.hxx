#pragma once

#include "Map/Biome.hxx"
#include "Map/PerlinNoise.hxx"
#include "Tiles/Tile.hxx"

class Map
{
  private:
    sf::Vector3<unsigned int> dimensions;
    sf::Texture &texturePack;
    unsigned int gridSize;
    float scale;

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
    Map(const sf::Vector3<unsigned int> dimensions, sf::Texture &texture_pack, const unsigned int &grid_size,
        const float &scale);

    ~Map();

    const sf::Vector3<unsigned int> &getDimensions() const;

    void generate();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};