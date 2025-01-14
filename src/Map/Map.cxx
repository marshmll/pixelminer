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

    saveToFile("Assets/Maps/myworld");
    // loadFromFile("Assets/Maps/myworld");
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
                biome_color = sf::Color(26 * std::pow(3.f, (1.f - moisture + heat)), 135, 22, 255);
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
            default:
                break;
            };

            image.setPixel({x, y}, biome_color);

            Tile tile(tile_data.name, tile_data.id, texturePack, tile_data.textureRect, gridSize, {}, scale);
            biomeMap[x][y] = (BiomeData){biome_type, biome_color};

            if (tile_data.id == TileId::GrassTop)
                tile.setColor(biome_color);

            putTile(tile, x, y, 0);
        }
    }

    (void)image.saveToFile("Assets/map.png");

    // // Grass curves
    // for (unsigned int y = 0; y < MAX_WORLD_GRID_SIZE.y; y++)
    // {
    //     for (unsigned int x = 0; x < MAX_WORLD_GRID_SIZE.x; x++)
    //     {
    //         std::optional<TileBase> tile = getTile(x, y, 0);
    //         if (!tile)
    //             continue;

    //         BiomeData biome_data = biomeMap[x][y];

    //         if (biome_data.type == Forest || biome_data.type == Grassland || biome_data.type == Jungle ||
    //             biome_data.type == Tundra)
    //         {
    //             std::array<std::array<BiomeData, 3>, 3> surround_biomes;

    //             for (int i = -1; i <= 1; i++)
    //             {
    //                 for (int j = -1; j <= 1; j++)
    //                 {
    //                     if (x + i >= 0 && x + i <= MAX_WORLD_GRID_SIZE.x && y + j >= 0 &&
    //                         y + j <= MAX_WORLD_GRID_SIZE.y)
    //                         surround_biomes[i + 1][j + 1] = biomeMap[x + i][y + j];
    //                     else
    //                         surround_biomes[i + 1][j + 1] = (BiomeData){UnknownBiome, sf::Color::Black};
    //                 }
    //             }

    //             TileData t;
    //             sf::Color c;

    //             if (surround_biomes[1][0].type != surround_biomes[1][1].type &&
    //                 surround_biomes[2][1].type != surround_biomes[1][1].type &&
    //                 surround_biomes[1][2].type == surround_biomes[1][1].type &&
    //                 surround_biomes[0][1].type == surround_biomes[1][1].type)
    //             {
    //                 t = tileData.at(GrassTopFront);
    //                 c = surround_biomes[2][0].color;
    //                 Tile tile(t.name, t.id, texturePack, t.textureRect, gridSize, {}, scale);
    //                 tile.setColor(c);
    //                 putTile(tile, x, y, 1);
    //             }
    //             else if (surround_biomes[2][0].type != surround_biomes[1][1].type &&
    //                      surround_biomes[2][1].type != surround_biomes[1][1].type &&
    //                      surround_biomes[2][2].type != surround_biomes[1][1].type)
    //             {
    //                 t = tileData.at(GrassFront);
    //                 c = surround_biomes[2][1].color;
    //                 Tile tile(t.name, t.id, texturePack, t.textureRect, gridSize, {}, scale);
    //                 tile.setColor(c);
    //                 putTile(tile, x, y, 1);
    //             }
    //         }
    //     }
    // }
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

void Map::render(sf::RenderTarget &target, const sf::Vector2i &entity_pos_grid)
{
    if (entity_pos_grid.x < 0 || entity_pos_grid.y < 0 || entity_pos_grid.x > MAX_WORLD_GRID_SIZE.x ||
        entity_pos_grid.y > MAX_WORLD_GRID_SIZE.y)
        return;

    const unsigned int region_index_x = entity_pos_grid.x / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x);
    const unsigned int region_index_y = entity_pos_grid.y / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y);

    const unsigned int chunk_index_x =
        ((entity_pos_grid.x - (region_index_x * REGION_SIZE_IN_CHUNKS.x)) / CHUNK_SIZE_IN_TILES.x) %
        REGION_SIZE_IN_CHUNKS.x;
    const unsigned int chunk_index_y =
        ((entity_pos_grid.y - (region_index_y * REGION_SIZE_IN_CHUNKS.y)) / CHUNK_SIZE_IN_TILES.y) %
        REGION_SIZE_IN_CHUNKS.y;

    // std::cout << chunk_index_x << " " << chunk_index_y << "\n";

    if (regions[region_index_x][region_index_y])
        if (regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y])
            regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y]->render(target, true);
}

void Map::putTile(TileBase tile, const unsigned int &grid_x, const unsigned int &grid_y, const unsigned int &grid_z)
{
    const unsigned int region_index_x = grid_x / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x);
    const unsigned int region_index_y = grid_y / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y);

    const unsigned int chunk_index_x =
        ((grid_x - (region_index_x * REGION_SIZE_IN_CHUNKS.x)) / CHUNK_SIZE_IN_TILES.x) % REGION_SIZE_IN_CHUNKS.x;
    const unsigned int chunk_index_y =
        ((grid_y - (region_index_y * REGION_SIZE_IN_CHUNKS.y)) / CHUNK_SIZE_IN_TILES.y) % REGION_SIZE_IN_CHUNKS.y;

    const unsigned int tile_index_x = (grid_x - (chunk_index_x * CHUNK_SIZE_IN_TILES.x)) % CHUNK_SIZE_IN_TILES.x;
    const unsigned int tile_index_y = (grid_y - (chunk_index_y * CHUNK_SIZE_IN_TILES.y)) % CHUNK_SIZE_IN_TILES.y;

    // std::cout << chunk_index_x << " " << chunk_index_y << "\n";

    tile.setGridPosition({grid_x, grid_y});

    if (!regions[region_index_x][region_index_y])
    {
        regions[region_index_x][region_index_y] =
            std::make_unique<Region>(sf::Vector2u(region_index_x, region_index_y), gridSize, scale);
    }

    if (!regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y])
    {
        regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y] =
            std::make_unique<Chunk>(sf::Vector2u(chunk_index_x + (region_index_x * REGION_SIZE_IN_CHUNKS.x),
                                                 chunk_index_y + (region_index_y * REGION_SIZE_IN_CHUNKS.y)),
                                    gridSize, scale);
    }

    regions[region_index_x][region_index_y]
        ->chunks[chunk_index_x][chunk_index_y]
        ->tiles[tile_index_x][tile_index_y][grid_z] = std::make_unique<TileBase>(tile);
}

std::optional<TileBase> Map::getTile(const int &grid_x, const int &grid_y, const int &grid_z)
{
    if (grid_x < 0 || grid_y < 0 || grid_z < 0 || grid_x >= MAX_WORLD_GRID_SIZE.x || grid_y >= MAX_WORLD_GRID_SIZE.y ||
        grid_z > CHUNK_SIZE_IN_TILES.z)
        return std::nullopt;

    const unsigned int region_index_x = grid_x / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x);
    const unsigned int region_index_y = grid_y / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y);

    const unsigned int chunk_index_x =
        ((grid_x - (region_index_x * REGION_SIZE_IN_CHUNKS.x)) / CHUNK_SIZE_IN_TILES.x) % REGION_SIZE_IN_CHUNKS.x;
    const unsigned int chunk_index_y =
        ((grid_y - (region_index_y * REGION_SIZE_IN_CHUNKS.y)) / CHUNK_SIZE_IN_TILES.y) % REGION_SIZE_IN_CHUNKS.y;

    const unsigned int tile_index_x = (grid_x - (chunk_index_x * CHUNK_SIZE_IN_TILES.x)) % CHUNK_SIZE_IN_TILES.x;
    const unsigned int tile_index_y = (grid_y - (chunk_index_y * CHUNK_SIZE_IN_TILES.y)) % CHUNK_SIZE_IN_TILES.y;

    if (!regions[region_index_x][region_index_y])
        return std::nullopt;

    if (!regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y])
        return std::nullopt;

    if (regions[region_index_x][region_index_y]->chunks[chunk_index_x][chunk_index_y]->tiles[tile_index_x][tile_index_y]
                                                                                            [grid_z])
    {
        return *regions[region_index_x][region_index_y]
                    ->chunks[chunk_index_x][chunk_index_y]
                    ->tiles[tile_index_x][tile_index_y][grid_z];
    }

    return std::nullopt;
}
void Map::saveToFile(std::filesystem::path path)
{
    std::string path_str = path.string();

    if (path_str.back() != '/')
        path_str.push_back('/');

    if (!std::filesystem::exists(path_str))
        std::filesystem::create_directory(path_str);

    if (!std::filesystem::exists(path_str + "regions"))
        std::filesystem::create_directory(path_str + "regions");

    for (auto &row : regions)
    {
        for (auto &region : row)
        {
            if (!region)
                continue;

            std::string fname =
                "r." + std::to_string(region->regionIndex.x) + "." + std::to_string(region->regionIndex.y) + ".region";

            std::ofstream region_file(path_str + "regions/" + fname, std::ios::binary);

            std::cout << path_str + "regions/" + fname << "\n";

            if (!region_file.is_open())
                throw std::runtime_error("[ Map::saveToFile ] -> Could not write region file: " + path_str +
                                         "regions/" + fname + "\n");

            for (unsigned short chunk_offset_x = 0; chunk_offset_x < REGION_SIZE_IN_CHUNKS.x; chunk_offset_x++)
            {
                for (unsigned short chunk_offset_y = 0; chunk_offset_y < REGION_SIZE_IN_CHUNKS.y; chunk_offset_y++)
                {
                    if (!region->chunks[chunk_offset_x][chunk_offset_y])
                        continue;

                    Chunk *chunk = region->chunks[chunk_offset_x][chunk_offset_y].get();
                    unsigned int tile_amount = 0;

                    for (auto &row : chunk->tiles)
                    {
                        for (auto &col : row)
                        {
                            for (auto &tile : col)
                            {
                                if (tile)
                                    tile_amount++;
                            }
                        }
                    }

                    region_file.write(reinterpret_cast<char *>(&chunk_offset_x), sizeof(unsigned short));
                    region_file.write(reinterpret_cast<char *>(&chunk_offset_y), sizeof(unsigned short));
                    region_file.write(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned int));

                    for (unsigned short x = 0; x < chunk->tiles.max_size(); x++)
                    {
                        for (unsigned short y = 0; y < chunk->tiles[x].max_size(); y++)
                        {
                            for (unsigned short z = 0; z < chunk->tiles[x][y].max_size(); z++)
                            {
                                if (!chunk->tiles[x][y][z])
                                    continue;

                                TileBase *tile = chunk->tiles[x][y][z].get();
                                std::uint32_t id = tile->getId();

                                region_file.write(reinterpret_cast<char *>(&x), sizeof(unsigned short));
                                region_file.write(reinterpret_cast<char *>(&y), sizeof(unsigned short));
                                region_file.write(reinterpret_cast<char *>(&z), sizeof(unsigned short));
                                region_file.write(reinterpret_cast<char *>(&id), sizeof(std::uint32_t));

                                // Terrain grass colors
                                if (id == TileId::GrassTop)
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
                throw std::runtime_error("[ Map::saveToFile ] -> ERROR: Unable to write to file: " + path.string() +
                                         "\n");

            region_file.close();
        }
    }

    std::cout << "[ Map::saveToFile ] -> Map saved to: " << path.string() << "\n";
}

void Map::loadFromFile(std::filesystem::path path)
{
    std::string path_str = path.string();

    if (path_str.back() != '/')
        path_str.push_back('/');

    if (!std::filesystem::exists(path_str))
        throw std::runtime_error("[ Map::loadFromFile ] -> Inexistent world: " + path.string() + "\n");

    if (!std::filesystem::exists(path_str + "regions/"))
        throw std::runtime_error("[ Map::loadFromFile ] -> Invalid world: " + path.string() + "\n");

    std::stringstream fname;

    for (unsigned int i = 0; i < MAX_REGIONS.x; i++)
    {
        for (unsigned int j = 0; j < MAX_REGIONS.y; j++)
        {
            fname.clear();
            fname << "r." << i << "." << j << ".region";

            if (!std::filesystem::exists(path_str + "regions/" + fname.str())) // Check if region exists
                continue;

            std::ifstream region_file(path_str + "regions/" + fname.str(), std::ios::binary);
            if (!region_file.is_open())
                throw std::runtime_error("[ Map::loadFromFile ] -> Could not read region file: " + path_str +
                                         "regions/" + fname.str() + "\n");

            regions[i][j] = std::make_unique<Region>(sf::Vector2u(i, j), gridSize, scale);

            for (int k = 0; k < REGION_SIZE_IN_CHUNKS.x * REGION_SIZE_IN_CHUNKS.y; k++)
            {
                unsigned short chunk_offset_x = 0, chunk_offset_y = 0;
                unsigned int tile_amount = 0;

                region_file.read(reinterpret_cast<char *>(&chunk_offset_x), sizeof(unsigned short));
                region_file.read(reinterpret_cast<char *>(&chunk_offset_y), sizeof(unsigned short));
                region_file.read(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned int));

                regions[i][j]->chunks[chunk_offset_x][chunk_offset_y] =
                    std::make_unique<Chunk>(sf::Vector2u(chunk_offset_x, chunk_offset_y), gridSize, scale);

                Chunk *new_chunk = regions[i][j]->chunks[chunk_offset_x][chunk_offset_y].get();

                for (int l = 0; l < tile_amount; l++)
                {
                    unsigned short x = 0, y = 0, z = 0;
                    std::uint32_t id;

                    region_file.read(reinterpret_cast<char *>(&x), sizeof(unsigned short));
                    region_file.read(reinterpret_cast<char *>(&y), sizeof(unsigned short));
                    region_file.read(reinterpret_cast<char *>(&z), sizeof(unsigned short));
                    region_file.read(reinterpret_cast<char *>(&id), sizeof(std::uint32_t));

                    sf::Vector2u grid_pos((i * REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x) +
                                              (chunk_offset_x * CHUNK_SIZE_IN_TILES.x) + x,
                                          (i * REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y) +
                                              (chunk_offset_y * CHUNK_SIZE_IN_TILES.y) + y);

                    TileData data;

                    if (tileData.count(id) == 0)
                        data = tileData.at(TileId::Unknown);
                    else
                        data = tileData.at(id);

                    new_chunk->tiles[x][y][z] = std::make_unique<Tile>(data.name, data.id, texturePack,
                                                                       data.textureRect, gridSize, grid_pos, scale);

                    if (id == TileId::GrassTop)
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

    std::cout << "[ Map::saveToFile ] -> Map loaded from: " << path.string() << "\n";
}

const sf::Vector2f &Map::getSpawnPoint() const
{
    return spawnPoint;
}
