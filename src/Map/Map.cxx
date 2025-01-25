#include "Map/Map.hxx"
#include "stdafx.hxx"

void Map::initRegionStatusArray()
{
    for (int x = 0; x < MAX_REGIONS.x; x++)
    {
        for (int y = 0; y < MAX_REGIONS.y; y++)
        {
            loadedRegions[x][y] = false;
        }
    }
}

void Map::initMetadata(const std::string &name, const long int &seed)
{
    metadata.metadataVersion = METADATA_VERSION;
    metadata.gameVersion = GAME_VERSION;
    metadata.creationTime = std::time(0);
    metadata.dataPacks.enabled = JSONArray({"vanilla"});
    metadata.dataPacks.disabled = JSONArray({});
    metadata.dayTime = 300000;
    metadata.difficulty = "normal";
    metadata.generatorName = "default";
    metadata.lastPlayed = -1;
    metadata.name = name;
    metadata.seed = seed;
    metadata.spawnX = static_cast<float>((rng.nextInt(0, INT32_MAX) % MAX_WORLD_GRID_SIZE.x) * gridSize);
    metadata.spawnY = static_cast<float>((rng.nextInt(0, INT32_MAX) % MAX_WORLD_GRID_SIZE.y) * gridSize);
    metadata.timePlayed = 0;
}

void Map::initTerrainGenerator(const long int &seed)
{
    terrainGenerator =
        std::make_unique<TerrainGenerator>(metadata, chunks, seed, texturePack, tileData, gridSize, scale);
}

Map::Map(const std::string &name, const long int &seed, std::map<std::uint32_t, TileData> &tile_data,
         sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale)

    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale), rng(seed)
{
    initRegionStatusArray();
    initMetadata(name, seed);
    initTerrainGenerator(seed);
}

Map::Map(std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack, const unsigned int &grid_size,
         const float &scale)
    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale), rng(0)
{
}

Map::~Map()
{
}

void Map::update(const float &dt, const sf::Vector2i &player_pos_grid)
{
    if (player_pos_grid.x < 0 || player_pos_grid.x > MAX_WORLD_GRID_SIZE.x || player_pos_grid.y < 0 ||
        player_pos_grid.y > MAX_WORLD_GRID_SIZE.y)
        return;

    const int REGION_X = player_pos_grid.x / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x);
    const int REGION_Y = player_pos_grid.y / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y);
    const int CHUNK_X = player_pos_grid.x / (CHUNK_SIZE_IN_TILES.x);
    const int CHUNK_Y = player_pos_grid.x / (CHUNK_SIZE_IN_TILES.x);

    // Load the region the player is in.
    if (!isRegionLoaded({REGION_X, REGION_Y}))
        loadRegion({REGION_X, REGION_Y});

    // If player is 1 chunk away from next chunk in the X axis
    if ((player_pos_grid.x + CHUNK_SIZE_IN_TILES.x) / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x) > REGION_X)
    {
        if (REGION_X + 1 > 0 && REGION_X + 1 < MAX_REGIONS.x)
            loadRegion({REGION_X + 1, REGION_Y});

        if (REGION_X - 1 > 0 && REGION_X - 1 < MAX_REGIONS.x)
            unloadRegion({REGION_X - 1, REGION_Y});
    }

    // If player is 1 chunk away from previous chunk in the X axis
    if ((player_pos_grid.x - CHUNK_SIZE_IN_TILES.x) / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x) < REGION_X)
    {
        if (REGION_X - 1 > 0 && REGION_X - 1 < MAX_REGIONS.x)
            loadRegion({REGION_X - 1, REGION_Y});

        if (REGION_X + 1 > 0 && REGION_X + 1 < MAX_REGIONS.x)
            unloadRegion({REGION_X + 1, REGION_Y});
    }

    // If player is 1 chunk away from next chunk in the Y axis
    if ((player_pos_grid.y + CHUNK_SIZE_IN_TILES.y) / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y) > REGION_Y)
    {
        if (REGION_Y + 1 > 0 && REGION_Y + 1 < MAX_REGIONS.y)
            loadRegion({REGION_X, REGION_Y + 1});

        if (REGION_Y - 1 > 0 && REGION_Y - 1 < MAX_REGIONS.y)
            unloadRegion({REGION_X, REGION_Y - 1});
    }

    // If player is 1 chunk away from previous chunk in the Y axis
    if ((player_pos_grid.y - CHUNK_SIZE_IN_TILES.y) / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y) < REGION_Y)
    {
        if (REGION_Y - 1 > 0 && REGION_Y - 1 < MAX_REGIONS.y)
            loadRegion({REGION_X, REGION_Y - 1});

        if (REGION_Y + 1 > 0 && REGION_Y + 1 < MAX_REGIONS.y)
            unloadRegion({REGION_X, REGION_Y + 1});
    }
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

    for (int i = 0; i < MAX_REGIONS.x; i++)
    {
        for (int j = 0; j < MAX_REGIONS.y; j++)
        {
            if (isRegionLoaded({i, j}))
            {
                saveRegion({i, j});
            }
        }
    }
}

void Map::save()
{
    // If no name provided, generate a random one.
    if (metadata.name.empty())
        metadata.name = std::to_string(std::rand());

    save(metadata.name);
}

void Map::saveRegion(const sf::Vector2i &region_index)
{
    if (region_index.x < 0 || region_index.x >= MAX_REGIONS.x || region_index.y < 0 || region_index.y >= MAX_REGIONS.y)
        std::cout << "Cannot save region (" << region_index.x << " " << region_index.y << "): Region out of bounds.";

    if (!isRegionLoaded(region_index))
        return;

    // Calculate chunk indexes.
    const int CHUNK_START_X = region_index.x * REGION_SIZE_IN_CHUNKS.x;
    const int CHUNK_START_Y = region_index.y * REGION_SIZE_IN_CHUNKS.y;
    const int CHUNK_END_X = (CHUNK_START_X + REGION_SIZE_IN_CHUNKS.x) - 1;
    const int CHUNK_END_Y = (CHUNK_START_Y + REGION_SIZE_IN_CHUNKS.y) - 1;

    // Check if region folder exists.
    if (!std::filesystem::exists(MAPS_FOLDER + metadata.name + "/regions/"))
    {
        std::cout << "Creating new directory: " << MAPS_FOLDER << metadata.name << "/regions/" << "\n";
        std::filesystem::create_directory(MAPS_FOLDER + metadata.name + "/regions/");
    }

    // Open file.
    std::string path;
    path = MAPS_FOLDER + metadata.name + "/regions/r." + std::to_string(region_index.x) + "." +
           std::to_string(region_index.y) + ".region";

    std::ofstream region_file(path, std::ios::binary);

    if (!region_file.is_open())
        throw std::runtime_error("Failed to write region file: " + path);

    unsigned long int total_tiles = 0;

    for (unsigned short c_x = CHUNK_START_X; c_x <= CHUNK_END_X; c_x++)
    {
        for (unsigned short c_y = CHUNK_START_Y; c_y <= CHUNK_END_Y; c_y++)
        {
            if (!chunks[c_x][c_y]) // Skip not-allocated chunks
                continue;

            // Count tiles in chunk
            unsigned short tile_amount = 0;
            for (auto &x : chunks[c_x][c_y]->tiles)
            {
                for (auto &y : x)
                {
                    for (auto &tile : y)
                    {
                        if (tile)
                            tile_amount++;
                    }
                }
            }

            total_tiles += tile_amount;

            region_file.write(reinterpret_cast<char *>(&c_x), sizeof(unsigned short));
            region_file.write(reinterpret_cast<char *>(&c_y), sizeof(unsigned short));
            region_file.write(reinterpret_cast<char *>(&chunks[c_x][c_y]->flags), sizeof(chunks[c_x][c_y]->flags));
            region_file.write(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned short));

            // std::cout << "Writing chunk_x: " << c_x << ", chunk_y: " << c_y
            //           << ", flags: " << static_cast<int>(chunks[c_x][c_y]->flags) << ", tile_amount: " << tile_amount
            //           << "\n";

            // Write tile data.
            for (unsigned short x = 0; x < CHUNK_SIZE_IN_TILES.x; x++)
            {
                for (unsigned short y = 0; y < CHUNK_SIZE_IN_TILES.y; y++)
                {
                    for (unsigned short z = 0; z < CHUNK_SIZE_IN_TILES.z; z++)
                    {
                        if (!chunks[c_x][c_y]->tiles[x][y][z])
                            continue;

                        uint32_t tile_id = chunks[c_x][c_y]->tiles[x][y][z]->getId();

                        region_file.write(reinterpret_cast<char *>(&x), sizeof(unsigned short));
                        region_file.write(reinterpret_cast<char *>(&y), sizeof(unsigned short));
                        region_file.write(reinterpret_cast<char *>(&z), sizeof(unsigned short));
                        region_file.write(reinterpret_cast<char *>(&tile_id), sizeof(uint32_t));

                        if (tile_id == TileId::GrassTile || tile_id == TileId::Grass1 || tile_id == TileId::Grass2)
                        {
                            sf::Color color = chunks[c_x][c_y]->tiles[x][y][z]->getColor();
                            region_file.write(reinterpret_cast<char *>(&color.r), sizeof(uint8_t));
                            region_file.write(reinterpret_cast<char *>(&color.g), sizeof(uint8_t));
                            region_file.write(reinterpret_cast<char *>(&color.b), sizeof(uint8_t));
                        }
                    }
                }
            }
        }
    }

    region_file.close();

    std::cout << "Written " << total_tiles << " tiles to region: " << path << "\n";
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

    // Reconfigure terraing generator.
    initTerrainGenerator(metadata.seed);
}

void Map::loadRegion(const sf::Vector2i &region_index)
{
    if (region_index.x < 0 || region_index.x >= MAX_REGIONS.x || region_index.y < 0 || region_index.y >= MAX_REGIONS.y)
        std::cout << "Cannot load region (" << region_index.x << " " << region_index.y << "): Region out of bounds.";

    if (isRegionLoaded(region_index))
        return;

    std::string path;
    path = MAPS_FOLDER + metadata.name + "/regions/r." + std::to_string(region_index.x) + "." +
           std::to_string(region_index.y) + ".region";

    if (!isRegionLoaded(region_index) && !std::filesystem::exists(path))
    {
        std::cout << "Generating region (" << region_index.x << ", " << region_index.y << ")..." << "\n";
        terrainGenerator->generateRegion(region_index);
        loadedRegions[region_index.x][region_index.y] = true;
        return;
    }

    std::ifstream region_file(path, std::ios::binary);
    if (!region_file.is_open())
        throw std::runtime_error("Failed to read region file: " + path);

    unsigned long total_tiles = 0;

    unsigned short chunk_x = 0, chunk_y = 0, tile_amount = 0;
    uint8_t flags;

    while (region_file.read(reinterpret_cast<char *>(&chunk_x), sizeof(unsigned short)) &&
           region_file.read(reinterpret_cast<char *>(&chunk_y), sizeof(unsigned short)) &&
           region_file.read(reinterpret_cast<char *>(&flags), sizeof(uint8_t)) &&
           region_file.read(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned short)))
    {
        // std::cout << "Reading chunk_x: " << chunk_x << ", chunk_y: " << chunk_y
        //           << ", flags: " << static_cast<int>(flags) << ", tile_amount: " << tile_amount << "\n";

        if (chunk_x < 0 || chunk_x >= MAX_CHUNKS.x || chunk_y < 0 || chunk_y >= MAX_CHUNKS.y)
            throw std::runtime_error("Corrupted region file: Chunk index out of bounds");

        if (chunks[chunk_x][chunk_y]) // Do not change loaded chunks
            continue;

        chunks[chunk_x][chunk_y] = std::make_unique<Chunk>(sf::Vector2u(chunk_x, chunk_y), gridSize, scale, flags);

        for (int i = 0; i < tile_amount; i++)
        {
            unsigned short x = 0, y = 0, z = 0; // Positions relative to chunk
            uint32_t tile_id;
            TileData td;

            region_file.read(reinterpret_cast<char *>(&x), sizeof(unsigned short));
            region_file.read(reinterpret_cast<char *>(&y), sizeof(unsigned short));
            region_file.read(reinterpret_cast<char *>(&z), sizeof(unsigned short));
            region_file.read(reinterpret_cast<char *>(&tile_id), sizeof(uint32_t));

            if (x > CHUNK_SIZE_IN_TILES.x || y > CHUNK_SIZE_IN_TILES.y || z > CHUNK_SIZE_IN_TILES.z)
                throw std::runtime_error("Corrupted region file: Tile nº" + std::to_string(i) +
                                         "out of bounds in Chunk[" + std::to_string(chunk_x) + "][" +
                                         std::to_string(chunk_y) + "]");

            td = tileData.at(tile_id);

            Tile tile(td.name, tile_id, texturePack, td.textureRect, gridSize,
                      sf::Vector2u(x + (chunk_x * CHUNK_SIZE_IN_TILES.x), y + (chunk_y * CHUNK_SIZE_IN_TILES.y)),
                      scale);

            // std::cout << "Tile placed at (" << x + (chunk_x * CHUNK_SIZE_IN_TILES.x) << ", "
            //           << y + (chunk_y * CHUNK_SIZE_IN_TILES.y) << "\n";

            if (tile_id == TileId::GrassTile || tile_id == TileId::Grass1 || tile_id == TileId::Grass2)
            {
                sf::Color color;
                region_file.read(reinterpret_cast<char *>(&color.r), sizeof(uint8_t));
                region_file.read(reinterpret_cast<char *>(&color.g), sizeof(uint8_t));
                region_file.read(reinterpret_cast<char *>(&color.b), sizeof(uint8_t));
                tile.setColor(color);
            }

            chunks[chunk_x][chunk_y]->tiles[x][y][z] = std::make_unique<Tile>(tile);
        }

        total_tiles += tile_amount;
    }

    region_file.close();

    loadedRegions[region_index.x][region_index.y] = true;
    std::cout << "Read " << total_tiles << " tiles from region: " << path << "\n";
}

void Map::unloadRegion(const sf::Vector2i &region_index)
{
    if (!isRegionLoaded(region_index))
        return;

    // Calculate chunk indexes.
    const int CHUNK_START_X = region_index.x * REGION_SIZE_IN_CHUNKS.x;
    const int CHUNK_START_Y = region_index.y * REGION_SIZE_IN_CHUNKS.y;
    const int CHUNK_END_X = (CHUNK_START_X + REGION_SIZE_IN_CHUNKS.x) - 1;
    const int CHUNK_END_Y = (CHUNK_START_Y + REGION_SIZE_IN_CHUNKS.y) - 1;

    for (unsigned short c_x = CHUNK_START_X; c_x <= CHUNK_END_X; c_x++)
    {
        for (unsigned short c_y = CHUNK_START_Y; c_y <= CHUNK_END_Y; c_y++)
        {
            if (chunks[c_x][c_y])
            {
                if ((chunks[c_x][c_y]->flags & ChunkFlags::KeepLoaded) ||
                    (chunks[c_x][c_y]->flags & ChunkFlags::Modified))
                    continue;

                chunks[c_x][c_y].reset();
            }
        }
    }

    loadedRegions[region_index.x][region_index.y] = false;
    std::cout << "Region \"r." << region_index.x << "." << region_index.y << ".region\" unloaded from memory." << "\n";
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

const sf::Vector2f Map::getSpawnPoint() const
{
    return sf::Vector2f(metadata.spawnX, metadata.spawnY);
}

const sf::Vector2f Map::getRealDimensions() const
{
    return sf::Vector2f(MAX_WORLD_GRID_SIZE.x * gridSize * scale, MAX_WORLD_GRID_SIZE.y * gridSize * scale);
}

const bool Map::isRegionLoaded(const sf::Vector2i &region_index) const
{
    if (region_index.x < 0 || region_index.x >= MAX_REGIONS.x || region_index.y < 0 || region_index.y >= MAX_REGIONS.y)
        throw std::runtime_error("Cannot check if region is loaded: region out of bounds (" +
                                 std::to_string(region_index.x) + ", " + std::to_string(region_index.y) + ")");

    return loadedRegions[region_index.x][region_index.y];
}