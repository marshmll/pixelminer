#include "Map/Map.hxx"
#include "stdafx.hxx"

void Map::resize()
{
    regions.resize(MAX_REGIONS.x);

    for (auto &row : regions)
        row.resize(MAX_REGIONS.y);
}

void Map::initPerlinWaves()
{
    height_waves = {{120.f, .009f, 4.f}, {300.f, .2f, 1.5f}, {500.f, .018f, 8.f}};
    moisture_waves = {{622.f, .04f, 5.f}, {200.f, .08f, 2.f}, {400.f, .2f, .8f}};
    heat_waves = {{318.6f, .05f, 5.f}, {329.7f, .5f, 1.f}};
}

void Map::initNoiseMaps()
{
    height_map = noise->generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .08f, height_waves, {0.f, 0.f});
    moisture_map =
        noise->generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .18f, moisture_waves, {10.f, 10.f});
    heat_map = noise->generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .08f, heat_waves, {5.f, 5.f});
}

void Map::initBiomes()
{
    biomes = {
        {BiomeType::Desert, .35f, 0.1f, .8f},  {BiomeType::Forest, .4f, .6f, .4f},
        {BiomeType::Grassland, .3f, .5f, .5f}, {BiomeType::Jungle, .45f, .8f, .7f},
        {BiomeType::Mountains, .9f, .3f, .3f}, {BiomeType::Ocean, .35f, .7f, .4f},
        {BiomeType::Tundra, .8f, .3f, .1f},
    };
}

void Map::randomizeSpawnPoint()
{
    spawnPoint.x = static_cast<float>((std::rand() % MAX_WORLD_GRID_SIZE.x) * gridSize);
    spawnPoint.y = static_cast<float>((std::rand() % MAX_WORLD_GRID_SIZE.y) * gridSize);
}

Map::Map(std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack, const unsigned int &grid_size,
         const float &scale, const long int &seed)

    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale), seed(seed),
      noise(std::make_unique<PerlinNoise>(seed))
{
    resize();
    initPerlinWaves();
    initNoiseMaps();
    initBiomes();
    generate();
    randomizeSpawnPoint();

    // saveToFile("Assets/Maps/mymap.map");
    // loadFromFile("Assets/Maps/mymap.map");
}

Map::Map(const std::filesystem::path path, std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack,
         const unsigned int &grid_size, const float &scale)
    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale)
{
    loadFromFile(path);
}

Map::~Map()
{
}

const sf::Vector2f Map::getRealDimensions() const
{
    return sf::Vector2f(MAX_WORLD_GRID_SIZE.x * gridSize * scale, MAX_WORLD_GRID_SIZE.y * gridSize * scale);
}

void Map::generate()
{
    sf::Image image({MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y});

    // Smooth biome transitions
    for (unsigned int y = 0; y < MAX_WORLD_GRID_SIZE.y; y++)
    {
        for (unsigned int x = 0; x < MAX_WORLD_GRID_SIZE.x; x++)
        {
            float height = height_map[x][y];
            float moisture = moisture_map[x][y];
            float heat = heat_map[x][y];

            // Weighted selection of biomes
            float max_weight = -1.f;
            BiomeType biome_type;
            sf::Color biome_color;
            TileData tile_data;

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
                tile_data = tileData.at(TileId::Sand);
                biome_color = sf::Color(194, 178, 128, 255);
                break;

            case Forest:
                biome_color = sf::Color(29 * std::pow(3.f, (1.f - moisture + heat)), 143, 27, 255);
                tile_data = tileData.at(TileId::GrassTop);
                break;

            case Grassland:
                biome_color = sf::Color(26 * std::pow(3.2f, (1.f - moisture + heat)), 148, 24, 255);
                tile_data = tileData.at(TileId::GrassTop);
                break;

            case Jungle:
                biome_color = sf::Color(25 * std::pow(3.2f, (1.f - moisture + heat)), 130, 20, 255);
                tile_data = tileData.at(TileId::GrassTop);
                break;

            case Mountains:
                biome_color = sf::Color(150, 150, 150, 255);
                tile_data = tileData.at(TileId::Stone);
                break;

            case Ocean:
                biome_color = sf::Color(16, 51, 163, 255);
                tile_data = tileData.at(TileId::Water);
                break;

            case Tundra:
                biome_color = sf::Color(216, 242, 230, 255);
                tile_data = tileData.at(TileId::Snow);
                break;
            };

            image.setPixel({x, y}, biome_color);

            Tile tile(tile_data.name, tile_data.id, texturePack, tile_data.textureRect, gridSize, {}, scale);

            if (tile_data.id == TileId::GrassTop)
                tile.setColor(biome_color);

            putTile(tile, x, y, 0);
        }
    }

    (void)image.saveToFile("Assets/map.png");
}

void Map::update(const float &dt)
{
}

void Map::render(sf::RenderTarget &target, const bool show_regions_and_chunks)
{
    for (auto &row : regions)
    {
        for (auto &region : row)
        {
            if (region)
                region->render(target, show_regions_and_chunks);
        }
    }
}

void Map::putTile(TileBase tile, const unsigned int &grid_x, const unsigned int &grid_y, const unsigned int &grid_z)
{
    const unsigned int region_index_x = grid_x / (REGION_SIZE.x * CHUNK_SIZE.x);
    const unsigned int region_index_y = grid_y / (REGION_SIZE.y * CHUNK_SIZE.y);

    const unsigned int chunk_index_x = (grid_x - (region_index_x * REGION_SIZE.x)) / CHUNK_SIZE.x;
    const unsigned int chunk_index_y = (grid_y - (region_index_y * REGION_SIZE.y)) / CHUNK_SIZE.y;

    const unsigned int tile_index_x = (grid_x - (chunk_index_x * CHUNK_SIZE.x));
    const unsigned int tile_index_y = (grid_y - (chunk_index_y * CHUNK_SIZE.y));

    // std::cout << region_index_x << " " << region_index_y << "\n";

    tile.setGridPosition({grid_x, grid_y});

    if (!regions[region_index_x][region_index_y])
    {
        regions[region_index_x][region_index_y] =
            std::make_unique<Region>(sf::Vector2u(region_index_x, region_index_y), gridSize, scale);
    }

    if (!regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y])
    {
        regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y] =
            std::make_unique<Chunk>(sf::Vector2u(chunk_index_x, chunk_index_y), gridSize, scale);
    }

    regions[region_index_x][region_index_y]
        ->chunks[chunk_index_x][chunk_index_y]
        ->tiles[tile_index_x][tile_index_y][grid_z] = std::make_unique<TileBase>(tile);
}

void Map::saveToFile(std::filesystem::path path)
{
    std::ofstream out(path);

    if (!out.is_open())
        throw std::runtime_error("[ Map::saveToFile ] -> ERROR: Unable to write to file: " + path.string() + "\n");

    std::cout << "[ Map::saveToFile ] -> Map saved to: " << path.string() << "\n";

    out.close();
}

void Map::loadFromFile(std::filesystem::path path)
{
    std::ifstream in(path);

    if (!in.is_open())
        throw std::runtime_error("[ Map::loadFromFile ] -> ERROR: Unable to load from file: " + path.string() + "\n");

    std::cout << "[ Map::saveToFile ] -> Map loaded from: " << path.string() << "\n";

    in.close();
}

const sf::Vector2f &Map::getSpawnPoint() const
{
    return spawnPoint;
}
