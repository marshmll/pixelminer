#include "Map/Map.hxx"
#include "stdafx.hxx"

void Map::clear()
{
    for (auto &x : tiles)
    {
        for (auto &y : x)
            y.clear();

        x.clear();
    }

    tiles.clear();
}

void Map::resize(const sf::Vector3<unsigned int> &dimensions)
{
    tiles.reserve(dimensions.x);

    for (auto &x : tiles)
    {
        x.reserve(dimensions.y);

        for (auto &y : x)
            y.reserve(dimensions.z);
    }
}

void Map::initPerlinWaves()
{
    height_waves = {{120.f, .009f, 4.f}, {300.f, .2f, 1.5f}, {500.f, .018f, 8.f}};
    moisture_waves = {{622.f, .04f, 5.f}, {200.f, .08f, 2.f}, {400.f, .2f, .8f}};
    heat_waves = {{318.6f, .05f, 5.f}, {329.7f, .5f, 1.f}};
}

void Map::initNoiseMaps()
{
    height_map = noise.generateNoiseMap(dimensions.x, dimensions.y, .5f, height_waves, {0.f, 0.f});
    moisture_map = noise.generateNoiseMap(dimensions.x, dimensions.y, .7f, moisture_waves, {10.f, 10.f});
    heat_map = noise.generateNoiseMap(dimensions.x, dimensions.y, .5f, heat_waves, {5.f, 5.f});
}

void Map::initBiomes()
{
    biomes = {
        {BiomeType::Desert, .2f, 0.1f, .8f},   {BiomeType::Forest, .4f, .6f, .4f},
        {BiomeType::Grassland, .3f, .5f, .5f}, {BiomeType::Jungle, .45f, .8f, .7f},
        {BiomeType::Mountains, .9f, .3f, .3f}, {BiomeType::Ocean, .35f, .7f, .4f},
        {BiomeType::Tundra, .8f, .3f, .1f},
    };
}

Map::Map(const sf::Vector3<unsigned int> dimensions) : dimensions(dimensions)
{
    // clear();
    // resize(dimensions);
    initPerlinWaves();
    initNoiseMaps();
    initBiomes();
    generate();
}

Map::~Map()
{
}

const sf::Vector3<unsigned int> &Map::getDimensions() const
{
    return dimensions;
}

void Map::generate()
{
    sf::Image image({dimensions.x, dimensions.y});

    // Smooth biome transitions
    for (unsigned int y = 0; y < dimensions.y; y++)
    {
        for (unsigned int x = 0; x < dimensions.x; x++)
        {
            float height = height_map[x][y];
            float moisture = moisture_map[x][y];
            float heat = heat_map[x][y];

            // Weighted selection of biomes
            float max_weight = -1.f;
            BiomeType biome_type;
            sf::Color biome_color;

            for (const auto &biome : biomes)
            {
                float weight = biome.calculateWeight(height, moisture, heat);
                if (weight > max_weight)
                {
                    max_weight = weight;
                    biome_type = biome.getType();
                }
            }

            switch (biome_type)
            {
            case Desert:
                biome_color = sf::Color(196, 178, 94, 255);
                break;
            case Forest:
                biome_color = sf::Color(29, 110, 29, 255);
                break;
            case Grassland:
                biome_color = sf::Color(22, 148, 22, 255);
                break;
            case Jungle:
                biome_color = sf::Color(2, 31, 8, 255);
                break;
            case Mountains:
                biome_color = sf::Color(91, 99, 93, 255);
                break;
            case Ocean:
                biome_color = sf::Color(16, 51, 163);
                break;
            case Tundra:
                biome_color = sf::Color(216, 242, 230, 255);
                break;
            };

            image.setPixel({x, y}, biome_color);
        }
    }

    (void)image.saveToFile("Assets/map.png");
}