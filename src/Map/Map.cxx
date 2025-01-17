#include "Map/Map.hxx"
#include "Map/Chunk.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/LinearCongruentialGenerator.hxx"
#include "stdafx.hxx"

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
    metadata.spawnX = static_cast<float>((std::rand() % MAX_WORLD_GRID_SIZE.x) * gridSize);
    metadata.spawnY = static_cast<float>((std::rand() % MAX_WORLD_GRID_SIZE.y) * gridSize);
}

Map::Map(std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack, const unsigned int &grid_size,
         const float &scale, const long int &seed)

    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale),
      noise(std::make_unique<PerlinNoise>(seed)), rng(seed)
{
    metadata.seed = seed;

    // initPerlinWaves();
    // initNoiseMaps();
    // initBiomes();
    // generate();
    // randomizeSpawnPoint();

    load("1989380570");
}

Map::Map(const std::string &name, std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack,
         const unsigned int &grid_size, const float &scale)
    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale), rng(0)
{
    load(name);
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
    sf::Image image({MAX_WORLD_GRID_SIZE.x / 2, MAX_WORLD_GRID_SIZE.y / 2});

    // Smooth biome transitions
    for (unsigned int x = 0; x < MAX_WORLD_GRID_SIZE.x; x++)
    {
        for (unsigned int y = 0; y < MAX_WORLD_GRID_SIZE.y; y++)
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
                biome_color = sf::Color(24 * std::pow(3.f, (1.f - moisture + heat)), 110, 20, 255);
                tile_data = tileData.at(TileId::GrassTile);
                break;

            case Grassland:
                biome_color = sf::Color(26 * std::pow(3.2f, (1.f - moisture + heat)), 148, 24, 255);
                tile_data = tileData.at(TileId::GrassTile);
                break;

            case Jungle:
                biome_color = sf::Color(25 * std::pow(3.2f, (1.f - moisture + heat)), 130, 20, 255);
                tile_data = tileData.at(TileId::GrassTile);
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
            default:
                break;
            };

            image.setPixel({static_cast<unsigned>(x / 2), static_cast<unsigned>(y / 2)}, biome_color);

            biomeMap[x][y] = (BiomeData){biome_type, biome_color};
            if (tile_data.id == TileId::GrassTile)
            {
                putTile(Tile(tile_data.name, tile_data.id, texturePack, tile_data.textureRect, gridSize, {}, scale,
                             biome_color),
                        x, y, 0);

                if (!rng.nextInt(0, 100))
                {
                    TileData td = tileData.at(Grass1);
                    putTile(Tile(td.name, td.id, texturePack, td.textureRect, gridSize, {}, scale, biome_color), x, y,
                            1);
                }
                else if (!rng.nextInt(0, 3))
                {
                    TileData td = tileData.at(Grass2);
                    putTile(Tile(td.name, td.id, texturePack, td.textureRect, gridSize, {}, scale, biome_color), x, y,
                            1);
                }
            }
            else
            {
                putTile(Tile(tile_data.name, tile_data.id, texturePack, tile_data.textureRect, gridSize, {}, scale), x,
                        y, 0);
            }
        }
    }

    (void)image.saveToFile("Assets/map.png");

    // Gaussian blur

    float kernel[3][3] = {
        {1.f / 16.f, 1.f / 8.f, 1.f / 16.f},
        {1.f / 8.f, 1.f / 4.f, 1.f / 8.f},
        {1.f / 16.f, 1.f / 8.f, 1.f / 16.f},
    };

    for (int it = 0; it < 5; it++)
    {
        for (int x = 0; x < MAX_WORLD_GRID_SIZE.x; x++)
        {
            for (int y = 0; y < MAX_WORLD_GRID_SIZE.y; y++)
            {
                Tile *tile = getTile(x, y, 0);
                if (!tile)
                    continue;
                if (tile->getId() != TileId::GrassTile)
                    continue;

                float r, g, b;
                r = g = b = 0.f;

                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        if (x + i >= 0 && x + i < MAX_WORLD_GRID_SIZE.x && y + j >= 0 && y + j < MAX_WORLD_GRID_SIZE.y)
                        {
                            r += getTile(x + i, y + j, 0)->getColor().r * kernel[i + 1][j + 1];
                            g += getTile(x + i, y + j, 0)->getColor().g * kernel[i + 1][j + 1];
                            b += getTile(x + i, y + j, 0)->getColor().b * kernel[i + 1][j + 1];
                        }
                        else
                        {
                            r += getTile(x, y, 0)->getColor().r * kernel[i + 1][j + 1];
                            g += getTile(x, y, 0)->getColor().g * kernel[i + 1][j + 1];
                            b += getTile(x, y, 0)->getColor().b * kernel[i + 1][j + 1];
                        }
                    }
                }

                tile->setColor(sf::Color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g),
                                         static_cast<std::uint8_t>(b), 255));
            }
        }
    }
}

void Map::update(const float &dt)
{
}

void Map::render(sf::RenderTarget &target, const bool &debug)
{
    for (auto &row : chunks)
    {
        for (auto &chunk : row)
        {
            if (chunk)
                chunk->render(target, debug);
        }
    }
}

void Map::render(sf::RenderTarget &target, const sf::Vector2i &entity_pos_grid, const bool &debug)
{
    const int chunk_x = entity_pos_grid.x / CHUNK_SIZE_IN_TILES.x;
    const int chunk_y = entity_pos_grid.y / CHUNK_SIZE_IN_TILES.y;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (chunk_x + i < 0 || chunk_y + j < 0 || chunk_x + i >= MAX_CHUNKS.x || chunk_y + j >= MAX_CHUNKS.y)
                continue;

            if (chunks[chunk_x + i][chunk_y + j])
                chunks[chunk_x + i][chunk_y + j]->render(target, debug);
        }
    }
}

void Map::putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z)
{
    if (grid_x < 0 || grid_x >= MAX_WORLD_GRID_SIZE.x || grid_y < 0 || grid_y >= MAX_WORLD_GRID_SIZE.y || grid_z < 0 ||
        grid_z >= CHUNK_SIZE_IN_TILES.z)
        return;

    const unsigned int chunk_x = grid_x / CHUNK_SIZE_IN_TILES.x;
    const unsigned int chunk_y = grid_y / CHUNK_SIZE_IN_TILES.y;

    const unsigned int tile_x = grid_x - (chunk_x * CHUNK_SIZE_IN_TILES.x);
    const unsigned int tile_y = grid_y - (chunk_y * CHUNK_SIZE_IN_TILES.y);

    tile.setGridPosition(sf::Vector2u(grid_x, grid_y));

    if (!chunks[chunk_x][chunk_y])
        chunks[chunk_x][chunk_y] = std::make_unique<Chunk>(sf::Vector2u(chunk_x, chunk_y), gridSize, scale);

    if (!chunks[chunk_x][chunk_y]->tiles[tile_x][tile_y][grid_z])
        chunks[chunk_x][chunk_y]->tiles[tile_x][tile_y][grid_z] = std::make_unique<Tile>(tile);
}

Tile *Map::getTile(const int &grid_x, const int &grid_y, const int &grid_z)
{
    if (grid_x < 0 || grid_y < 0 || grid_z < 0 || grid_x >= MAX_WORLD_GRID_SIZE.x || grid_y >= MAX_WORLD_GRID_SIZE.y ||
        grid_z > CHUNK_SIZE_IN_TILES.z)
        return nullptr;

    const unsigned int chunk_x = grid_x / CHUNK_SIZE_IN_TILES.x;
    const unsigned int chunk_y = grid_y / CHUNK_SIZE_IN_TILES.y;

    const unsigned int tile_x = grid_x - (chunk_x * CHUNK_SIZE_IN_TILES.x);
    const unsigned int tile_y = grid_y - (chunk_y * CHUNK_SIZE_IN_TILES.y);

    if (!chunks[chunk_x][chunk_y])
        return nullptr;

    if (!chunks[chunk_x][chunk_y]->tiles[tile_x][tile_y][grid_z])
        return nullptr;

    return chunks[chunk_x][chunk_y]->tiles[tile_x][tile_y][grid_z].get();
}

void Map::save(const std::string &name)
{
    std::string path_str = MAPS_FOLDER + name;

    if (path_str.back() != '/')
        path_str.push_back('/');

    if (!std::filesystem::exists(path_str))
        std::filesystem::create_directory(path_str);

    /* WORLD METADATA +++++++++++++++++++++++++++++++++++++++++++++++++ */

    JSONObject metadataObj;
    metadataObj << metadata;

    // std::cout << JSON::stringify(metadataObj) << "\n";

    std::ofstream metadataFile(path_str + "metadata.json");

    if (!metadataFile.is_open())
        throw std::runtime_error("[ Map::saveToFile ] -> Could not write region file: " + path_str + "metadata.json\n");

    metadataFile << JSON::stringify(metadataObj);
    metadataFile.close();

    /* REGIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    if (!std::filesystem::exists(path_str + "regions"))
        std::filesystem::create_directory(path_str + "regions");

    for (unsigned int region_x = 0; region_x < MAX_REGIONS.x; region_x++)
    {
        for (unsigned int region_y = 0; region_y < MAX_REGIONS.y; region_y++)
        {
            std::string fname = "r." + std::to_string(region_x) + "." + std::to_string(region_y) + ".region";

            // std::cout << path_str + "regions/" + fname << "\n";

            std::ofstream region_file(path_str + "regions/" + fname, std::ios::binary);

            if (!region_file.is_open())
                throw std::runtime_error("[ Map::saveToFile ] -> Could not write region file: " + path_str +
                                         "regions/" + fname + "\n");

            for (unsigned short chunk_x = region_x * REGION_SIZE_IN_CHUNKS.x;
                 chunk_x < (region_x * REGION_SIZE_IN_CHUNKS.x) + REGION_SIZE_IN_CHUNKS.x; chunk_x++)
            {
                for (unsigned short chunk_y = region_y * REGION_SIZE_IN_CHUNKS.y;
                     chunk_y < (region_y * REGION_SIZE_IN_CHUNKS.y) + REGION_SIZE_IN_CHUNKS.y; chunk_y++)
                {
                    if (!chunks[chunk_x][chunk_y])
                        continue;

                    Chunk *chunk = chunks[chunk_x][chunk_y].get();
                    unsigned int tile_amount = 0;

                    for (int x = 0; x < CHUNK_SIZE_IN_TILES.x; x++)
                    {
                        for (int y = 0; y < CHUNK_SIZE_IN_TILES.y; y++)
                        {
                            for (int z = 0; z < CHUNK_SIZE_IN_TILES.z; z++)
                            {
                                if (chunk->tiles[x][y][z].get())
                                    tile_amount++;
                            }
                        }
                    }

                    std::cout << tile_amount << "\n";

                    region_file.write(reinterpret_cast<char *>(&chunk_x), sizeof(unsigned short));
                    region_file.write(reinterpret_cast<char *>(&chunk_y), sizeof(unsigned short));
                    region_file.write(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned int));

                    for (unsigned short x = 0; x < CHUNK_SIZE_IN_TILES.x; x++)
                    {
                        for (unsigned short y = 0; y < CHUNK_SIZE_IN_TILES.y; y++)
                        {
                            for (unsigned short z = 0; z < CHUNK_SIZE_IN_TILES.z; z++)
                            {
                                if (!chunk->tiles[x][y][z].get())
                                    continue;

                                Tile *tile = chunk->tiles[x][y][z].get();
                                std::uint32_t id = tile->getId();

                                region_file.write(reinterpret_cast<char *>(&x), sizeof(unsigned short));
                                region_file.write(reinterpret_cast<char *>(&y), sizeof(unsigned short));
                                region_file.write(reinterpret_cast<char *>(&z), sizeof(unsigned short));
                                region_file.write(reinterpret_cast<char *>(&id), sizeof(std::uint32_t));

                                // Terrain grass colors
                                if (id == TileId::GrassTile)
                                {
                                    sf::Color color = tile->getColor();
                                    region_file.write(reinterpret_cast<char *>(&color.r), sizeof(std::uint8_t));
                                    region_file.write(reinterpret_cast<char *>(&color.g), sizeof(std::uint8_t));
                                    region_file.write(reinterpret_cast<char *>(&color.b), sizeof(std::uint8_t));
                                }
                            }
                        }
                    }
                }
            }

            if (!region_file.is_open())
                throw std::runtime_error("[ Map::saveToFile ] -> ERROR: Unable to write to file: " + path_str + "\n");

            region_file.close();
        }
    }

    std::cout << "[ Map::saveToFile ] -> Map saved to: " << path_str << "\n";
}

void Map::save()
{
    // If no name provided, generate a random one.
    if (metadata.name.empty())
        metadata.name = std::to_string(std::rand());

    save(metadata.name);
}

void Map::load(const std::string &name)
{
    std::string path_str = MAPS_FOLDER + name;

    if (path_str.back() != '/')
        path_str.push_back('/');

    if (!std::filesystem::exists(path_str))
        throw std::runtime_error("[ Map::loadFromFile ] -> Inexistent world: " + path_str + "\n");

    /* METADATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    if (!std::filesystem::exists(path_str + "metadata.json"))
        throw std::runtime_error("[ Map::loadFromFile ] -> No world metadata found: " + path_str + "\n");

    std::ifstream metadataFile(path_str + "metadata.json");

    if (!metadataFile.is_open())
        throw std::runtime_error("[ Map::loadFromFile ] -> Failed to open file: " + path_str + "metadata.json" + "\n");

    std::stringstream ss;
    ss << metadataFile.rdbuf();

    JSONObject metadataObj = JSON::parse(ss.str()).get<JSONObject>();
    metadataObj >> metadata;

    // std::cout << JSON::stringify(metadataObj) << "\n";

    /* REGIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    if (!std::filesystem::exists(path_str + "regions/"))
        throw std::runtime_error("[ Map::loadFromFile ] -> Invalid world: " + path_str + "\n");

    for (unsigned int i = 0; i < MAX_REGIONS.x; i++)
    {
        for (unsigned int j = 0; j < MAX_REGIONS.y; j++)
        {
            std::stringstream fname;
            fname << "r." << i << "." << j << ".region";

            if (!std::filesystem::exists(path_str + "regions/" + fname.str())) // Check if region exists
                continue;

            std::ifstream region_file(path_str + "regions/" + fname.str(), std::ios::binary);
            if (!region_file.is_open())
                throw std::runtime_error("[ Map::loadFromFile ] -> Could not read region file: " + path_str +
                                         "regions/" + fname.str() + "\n");

            unsigned short chunk_x = 0, chunk_y = 0;
            unsigned int tile_amount = 0;

            while (region_file.read(reinterpret_cast<char *>(&chunk_x), sizeof(unsigned short)) &&
                   region_file.read(reinterpret_cast<char *>(&chunk_y), sizeof(unsigned short)) &&
                   region_file.read(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned int)))
            {

                chunks[chunk_x][chunk_y] = std::make_unique<Chunk>(sf::Vector2u(chunk_x, chunk_y), gridSize, scale);

                Chunk *new_chunk = chunks[chunk_x][chunk_y].get();

                std::cout << tile_amount << "\n";

                for (int l = 0; l < tile_amount; l++)
                {
                    unsigned short x = 0, y = 0, z = 0;
                    std::uint32_t id;

                    region_file.read(reinterpret_cast<char *>(&x), sizeof(unsigned short));
                    region_file.read(reinterpret_cast<char *>(&y), sizeof(unsigned short));
                    region_file.read(reinterpret_cast<char *>(&z), sizeof(unsigned short));
                    region_file.read(reinterpret_cast<char *>(&id), sizeof(std::uint32_t));

                    TileData data;
                    sf::Vector2u grid_pos =
                        sf::Vector2u(x + (chunk_x * CHUNK_SIZE_IN_TILES.x), y + (chunk_y * CHUNK_SIZE_IN_TILES.y));

                    if (tileData.count(id) == 0)
                        data = tileData.at(TileId::Unknown);
                    else
                        data = tileData.at(id);

                    new_chunk->tiles[x][y][z] = std::make_unique<Tile>(data.name, data.id, texturePack,
                                                                       data.textureRect, gridSize, grid_pos, scale);

                    if (id == TileId::GrassTile)
                    {
                        sf::Color color;
                        region_file.read(reinterpret_cast<char *>(&color.r), sizeof(std::uint8_t));
                        region_file.read(reinterpret_cast<char *>(&color.g), sizeof(std::uint8_t));
                        region_file.read(reinterpret_cast<char *>(&color.b), sizeof(std::uint8_t));

                        new_chunk->tiles[x][y][z]->setColor(color);
                    }
                }
            }
        }
    }

    std::cout << "[ Map::loadFromFile ] -> Map loaded from: " << path_str << "\n";
}

const sf::Vector2f Map::getSpawnPoint() const
{
    return sf::Vector2f(metadata.spawnX, metadata.spawnY);
}
