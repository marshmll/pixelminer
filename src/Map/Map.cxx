#include "Map/Map.hxx"
#include "stdafx.hxx"

void Map::initRegionStatusArray()
{
    for (auto &row : loadedRegions)
    {
        for (auto &region : row)
            region = false;
    }
}

void Map::initMetadata(const std::string &name, const long int &seed)
{
    msg = "Generating world metadata...";

    metadata.metadataVersion = METADATA_VERSION;
    metadata.gameVersion = GAME_VERSION;
    metadata.creationDate = std::time(0);
    metadata.dataPacks.enabled = JArray({"vanilla"});
    metadata.dataPacks.disabled = JArray({});
    metadata.dayTime = 300000;
    metadata.difficulty = "normal";
    metadata.generatorName = "default";
    metadata.lastPlayed = -1;
    metadata.name = name;
    metadata.seed = seed;
    metadata.spawnX = static_cast<float>(rng.nextFloat() * MAX_WORLD_GRID_SIZE.x);
    metadata.spawnY = static_cast<float>(rng.nextFloat() * MAX_WORLD_GRID_SIZE.y);
    metadata.timePlayed = 0;
}

void Map::initTerrainGenerator(const long int &seed)
{
    std::lock_guard<std::mutex> lock(mutex);
    terrainGenerator =
        std::make_unique<TerrainGenerator>(msg, metadata, chunks, seed, texturePack, tileDB, gridSize, scale);
    setReady(true);
    clock.restart();
}

void Map::setReady(const bool ready)
{
    this->ready = ready;
}

Map::Map(const std::string &name, const long int &seed, std::unordered_map<std::string, TileData> &tile_db,
         sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale)
    : logger("Map"), ready(false), msg("Preparing to load"), folderName(name), tileDB(tile_db),
      texturePack(texture_pack), gridSize(grid_size), scale(scale), rng(seed)
{
    initRegionStatusArray();
    initMetadata(name, seed);
    std::thread(&Map::initTerrainGenerator, this, seed).detach();
}

Map::Map(std::unordered_map<std::string, TileData> &tile_db, sf::Texture &texture_pack, const unsigned int &grid_size,
         const float &scale)
    : logger("Map"), ready(false), msg("Preparing to load"), folderName("ERROR"), tileDB(tile_db),
      texturePack(texture_pack), gridSize(grid_size), scale(scale), rng(0)
{
    initRegionStatusArray();
}

Map::~Map() = default;

void Map::update(const float &dt, const sf::Vector2i &player_pos_grid)
{
    if (!isReady())
        return;

    if (player_pos_grid.x < 0 || player_pos_grid.x > MAX_WORLD_GRID_SIZE.x || player_pos_grid.y < 0 ||
        player_pos_grid.y > MAX_WORLD_GRID_SIZE.y)
        return;

    const int REGION_X = player_pos_grid.x / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x);
    const int REGION_Y = player_pos_grid.y / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y);
    std::optional<bool> loaded;

    loaded = isRegionLoaded({REGION_X, REGION_Y});
    if (loaded.has_value())
    {
        if (loaded.value() == false)
            std::thread(&Map::loadRegion, this, sf::Vector2i(REGION_X, REGION_Y)).detach();
    }

    auto manageRegions = [&](int load_offset_x, int load_offset_y, int unload_offset_x, int unload_offset_y) {
        sf::Vector2i load_region(REGION_X + load_offset_x, REGION_Y + load_offset_y);
        sf::Vector2i unload_region(REGION_X + unload_offset_x, REGION_Y + unload_offset_y);

        if (load_region.x >= 0 && load_region.x < MAX_REGIONS.x && load_region.y >= 0 && load_region.y < MAX_REGIONS.y)
        {
            loaded = isRegionLoaded(load_region);
            if (loaded.has_value())
            {
                if (!loaded.value())
                    std::thread(&Map::loadRegion, this, load_region).detach();
            }
        }

        if (unload_region.x >= 0 && unload_region.x < MAX_REGIONS.x && unload_region.y >= 0 &&
            unload_region.y < MAX_REGIONS.y)
        {
            loaded = isRegionLoaded(unload_region);
            if (loaded.has_value())
            {
                if (loaded.value())
                    std::thread(&Map::unloadRegion, this, unload_region).detach();
            }
        }
    };

    const int X_OFFSET = CHUNK_SIZE_IN_TILES.x;
    const int Y_OFFSET = CHUNK_SIZE_IN_TILES.y;

    if ((player_pos_grid.x + X_OFFSET) / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x) == REGION_X + 1)
    {
        manageRegions(1, -1, -2, -1);
        manageRegions(1, 0, -2, 0);
        manageRegions(1, 1, -2, 1);
    }

    if ((player_pos_grid.x - X_OFFSET) / (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x) == REGION_X - 1)
    {
        manageRegions(-1, -1, 2, -1);
        manageRegions(-1, 0, 2, 0);
        manageRegions(-1, 1, 2, 1);
    }

    if ((player_pos_grid.y + Y_OFFSET) / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y) == REGION_Y + 1)
    {
        manageRegions(-1, 1, -1, -2);
        manageRegions(0, 1, 0, -2);
        manageRegions(1, 1, 1, -2);
    }

    if ((player_pos_grid.y - Y_OFFSET) / (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y) == REGION_Y - 1)
    {
        manageRegions(-1, -1, -1, 2);
        manageRegions(0, -1, 0, 2);
        manageRegions(1, -1, 1, 2);
    }
}

void Map::render(sf::RenderTarget &target, const bool &debug)
{
    if (!isReady())
        return;

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
    if (!isReady())
        return;

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
    if (!isReady())
        return;

    std::string path_str = MAPS_FOLDER + name;
    if (path_str.back() != '/')
        path_str.push_back('/');

    if (!std::filesystem::exists(path_str))
        std::filesystem::create_directory(path_str);

    metadata.lastPlayed = std::time(0);
    metadata.timePlayed += clock.getElapsedTime().asSeconds();

    JObject metadataObj;
    metadataObj << metadata;

    std::ofstream metadataFile(path_str + "metadata.json");
    if (!metadataFile.is_open())
        logger.logError("[ Map::saveToFile ] -> Could not write region file: " + path_str + "metadata.json");

    metadataFile << JSON::stringify(metadataObj);
    metadataFile.close();

    for (int i = 0; i < MAX_REGIONS.x; i++)
    {
        for (int j = 0; j < MAX_REGIONS.y; j++)
        {
            if (isRegionLoaded({i, j}))
                saveRegion({i, j});
        }
    }
}

void Map::save()
{
    if (!isReady())
        return;

    if (metadata.name.empty())
        metadata.name = std::to_string(std::rand());
    save(metadata.name);
}

void Map::saveRegion(const sf::Vector2i &region_index)
{
    if (!isReady() || region_index.x < 0 || region_index.x >= MAX_REGIONS.x || region_index.y < 0 ||
        region_index.y >= MAX_REGIONS.y)
        return;

    if (!isRegionLoaded(region_index))
        return;

    const int CHUNK_START_X = region_index.x * REGION_SIZE_IN_CHUNKS.x;
    const int CHUNK_START_Y = region_index.y * REGION_SIZE_IN_CHUNKS.y;
    const int CHUNK_END_X = (CHUNK_START_X + REGION_SIZE_IN_CHUNKS.x) - 1;
    const int CHUNK_END_Y = (CHUNK_START_Y + REGION_SIZE_IN_CHUNKS.y) - 1;

    if (!std::filesystem::exists(MAPS_FOLDER + metadata.name + "/regions/"))
    {
        std::filesystem::create_directory(MAPS_FOLDER + metadata.name + "/regions/");
    }

    std::string path = MAPS_FOLDER + metadata.name + "/regions/r." + std::to_string(region_index.x) + "." +
                       std::to_string(region_index.y) + ".region";
    std::ofstream region_file(path, std::ios::binary);
    if (!region_file.is_open())
        logger.logError("Failed to write region file: " + path);

    unsigned long int total_tiles = 0;

    for (unsigned short c_x = CHUNK_START_X; c_x <= CHUNK_END_X; c_x++)
    {
        for (unsigned short c_y = CHUNK_START_Y; c_y <= CHUNK_END_Y; c_y++)
        {
            if (!chunks[c_x][c_y])
                continue;

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

            for (unsigned short x = 0; x < CHUNK_SIZE_IN_TILES.x; x++)
            {
                for (unsigned short y = 0; y < CHUNK_SIZE_IN_TILES.y; y++)
                {
                    for (unsigned short z = 0; z < CHUNK_SIZE_IN_TILES.z; z++)
                    {
                        if (!chunks[c_x][c_y]->tiles[x][y][z])
                            continue;

                        std::string id = chunks[c_x][c_y]->tiles[x][y][z]->getId();
                        uint8_t id_size = id.size();

                        region_file.write(reinterpret_cast<char *>(&id_size), sizeof(uint8_t));
                        region_file.write(id.data(), id.size());
                        region_file.write(reinterpret_cast<char *>(&x), sizeof(unsigned short));
                        region_file.write(reinterpret_cast<char *>(&y), sizeof(unsigned short));
                        region_file.write(reinterpret_cast<char *>(&z), sizeof(unsigned short));
                    }
                }
            }
        }
    }

    region_file.close();
    logger.logInfo("Written " + std::to_string(total_tiles) + " tiles to region: " + path);
}

void Map::load(const std::string &name)
{
    std::string path_str = MAPS_FOLDER + name;
    if (path_str.back() != '/')
        path_str.push_back('/');

    if (!std::filesystem::exists(path_str))
        logger.logError("[ Map::loadFromFile ] -> Inexistent world: " + path_str);

    msg = "Loading world metadata...";
    folderName = name;

    if (!std::filesystem::exists(path_str + "metadata.json"))
        logger.logError("[ Map::loadFromFile ] -> No world metadata found: " + path_str);

    std::ifstream metadataFile(path_str + "metadata.json");
    if (!metadataFile.is_open())
        logger.logError("[ Map::loadFromFile ] -> Failed to open file: " + path_str + "metadata.json");

    std::stringstream ss;
    ss << metadataFile.rdbuf();

    JObject metadataObj = JSON::parse(ss.str()).getAs<JObject>();
    metadataObj >> metadata;
    metadataFile.close();

    msg = "Initializing terrain generator...";
    std::thread(&Map::initTerrainGenerator, this, metadata.seed).detach();
}

void Map::loadRegion(const sf::Vector2i &region_index)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (!isReady() || region_index.x < 0 || region_index.x >= MAX_REGIONS.x || region_index.y < 0 ||
        region_index.y >= MAX_REGIONS.y)
        return;

    std::optional<bool> loaded = isRegionLoaded(region_index);
    if (loaded.has_value())
    {
        if (loaded.value() == true)
            return;
    }
    else
        return;

    std::string path = MAPS_FOLDER + metadata.name + "/regions/r." + std::to_string(region_index.x) + "." +
                       std::to_string(region_index.y) + ".region";

    if (!loaded.value() && !std::filesystem::exists(path))
    {
        terrainGenerator->generateRegion(region_index);
        loadedRegions[region_index.x][region_index.y] = true;
        return;
    }

    std::ifstream region_file(path, std::ios::binary);
    if (!region_file.is_open())
        logger.logError("Failed to read region file: " + path);

    unsigned long total_tiles = 0;
    unsigned short chunk_x = 0, chunk_y = 0, tile_amount = 0;
    uint8_t flags;

    while (region_file.read(reinterpret_cast<char *>(&chunk_x), sizeof(unsigned short)) &&
           region_file.read(reinterpret_cast<char *>(&chunk_y), sizeof(unsigned short)) &&
           region_file.read(reinterpret_cast<char *>(&flags), sizeof(uint8_t)) &&
           region_file.read(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned short)))
    {

        if (chunk_x < 0 || chunk_x >= MAX_CHUNKS.x || chunk_y < 0 || chunk_y >= MAX_CHUNKS.y)
            logger.logError("Corrupted region file: Chunk index out of bounds");

        if (!chunks[chunk_x][chunk_y])
        {
            chunks[chunk_x][chunk_y] = std::make_unique<Chunk>(sf::Vector2u(chunk_x, chunk_y), gridSize, scale, flags);
        }

        for (int i = 0; i < tile_amount; i++)
        {
            unsigned short x = 0, y = 0, z = 0;
            std::string id;
            uint8_t id_size;
            char buf[256];

            region_file.read(reinterpret_cast<char *>(&id_size), sizeof(uint8_t));
            region_file.read(buf, id_size);
            region_file.read(reinterpret_cast<char *>(&x), sizeof(unsigned short));
            region_file.read(reinterpret_cast<char *>(&y), sizeof(unsigned short));
            region_file.read(reinterpret_cast<char *>(&z), sizeof(unsigned short));

            id = std::string(buf, id_size);

            if (x > CHUNK_SIZE_IN_TILES.x || y > CHUNK_SIZE_IN_TILES.y || z > CHUNK_SIZE_IN_TILES.z)
                logger.logError("Corrupted region file: Tile[" + std::to_string(x) + "][" + std::to_string(y) + "][" +
                                std::to_string(z) + "] out of bounds in Chunk[" + std::to_string(chunk_x) + "][" +
                                std::to_string(chunk_y) + "]");

            TileData td;
            try
            {
                td = tileDB.at(id);
            }
            catch (std::out_of_range &)
            {
                logger.logError("Invalid tile ID: " + id + " Tile[" + std::to_string(x) + "][" + std::to_string(y) +
                                "][" + std::to_string(z) + "] out of bounds in Chunk[" + std::to_string(chunk_x) +
                                "][" + std::to_string(chunk_y) + "]");
            }

            sf::Vector2u grid_pos(x + (chunk_x * CHUNK_SIZE_IN_TILES.x), y + (chunk_y * CHUNK_SIZE_IN_TILES.y));
            BiomePreset biome_data = terrainGenerator->getBiomeData(grid_pos);

            Tile tile(td.name, id, texturePack, td.rect, gridSize, grid_pos, scale, biome_data.color);
            chunks[chunk_x][chunk_y]->tiles[x][y][z] = std::make_unique<Tile>(tile);
        }

        total_tiles += tile_amount;
    }

    region_file.close();
    loadedRegions[region_index.x][region_index.y] = true;
    logger.logInfo("Read " + std::to_string(total_tiles) + " tiles from region: " + path);
}

void Map::unloadRegion(const sf::Vector2i &region_index)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (!isReady())
        return;

    std::optional<bool> loaded = isRegionLoaded(region_index);
    if (loaded.has_value())
    {
        if (loaded.value() == false)
            return;
    }
    else
        return;

    const int CHUNK_START_X = region_index.x * REGION_SIZE_IN_CHUNKS.x;
    const int CHUNK_START_Y = region_index.y * REGION_SIZE_IN_CHUNKS.y;
    const int CHUNK_END_X = (CHUNK_START_X + REGION_SIZE_IN_CHUNKS.x) - 1;
    const int CHUNK_END_Y = (CHUNK_START_Y + REGION_SIZE_IN_CHUNKS.y) - 1;

    for (unsigned short c_x = CHUNK_START_X; c_x <= CHUNK_END_X; c_x++)
    {
        for (unsigned short c_y = CHUNK_START_Y; c_y <= CHUNK_END_Y; c_y++)
        {
            if (chunks[c_x][c_y] && !(chunks[c_x][c_y]->flags & ChunkFlags::KeepLoaded) &&
                !(chunks[c_x][c_y]->flags & ChunkFlags::Modified))
            {
                chunks[c_x][c_y].reset();
            }
        }
    }

    loadedRegions[region_index.x][region_index.y] = false;
    logger.logInfo("Region (" + std::to_string(region_index.x) + ", " + std::to_string(region_index.y) +
                   ") unloaded from memory.");
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

    if (!chunks[chunk_x][chunk_y] || !chunks[chunk_x][chunk_y]->tiles[tile_x][tile_y][grid_z])
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

const BiomePreset Map::getBiomeAt(const sf::Vector2i &grid_coords) const
{
    if (grid_coords.x < 0 || grid_coords.y < 0 || grid_coords.x >= MAX_WORLD_GRID_SIZE.x ||
        grid_coords.y >= MAX_WORLD_GRID_SIZE.y)
        return BiomePreset{};

    return this->terrainGenerator->getBiomeData(sf::Vector2u(grid_coords));
}

const float Map::getHeightAt(const sf::Vector2i &grid_pos) const
{
    if (grid_pos.x < 0 || grid_pos.y < 0 || grid_pos.x >= MAX_WORLD_GRID_SIZE.x || grid_pos.y >= MAX_WORLD_GRID_SIZE.y)
        return 0.f;

    return terrainGenerator->getHeightAt(sf::Vector2u(grid_pos));
}

const float Map::getMoistureAt(const sf::Vector2i &grid_pos) const
{
    if (grid_pos.x < 0 || grid_pos.y < 0 || grid_pos.x >= MAX_WORLD_GRID_SIZE.x || grid_pos.y >= MAX_WORLD_GRID_SIZE.y)
        return 0.f;

    return terrainGenerator->getMoistureAt(sf::Vector2u(grid_pos));
}

const float Map::getHeatAt(const sf::Vector2i &grid_pos) const
{
    if (grid_pos.x < 0 || grid_pos.y < 0 || grid_pos.x >= MAX_WORLD_GRID_SIZE.x || grid_pos.y >= MAX_WORLD_GRID_SIZE.y)
        return 0.f;

    return terrainGenerator->getHeatAt(sf::Vector2u(grid_pos));
}

const bool Map::isReady() const
{
    return ready;
}

const std::string &Map::getMessage() const
{
    return msg;
}

const std::string &Map::getFolderName() const
{
    return folderName;
}

const std::optional<bool> Map::isRegionLoaded(const sf::Vector2i &region_index)
{
    if (region_index.x < 0 || region_index.x >= MAX_REGIONS.x || region_index.y < 0 || region_index.y >= MAX_REGIONS.y)
        logger.logWarning("Region (" + std::to_string(region_index.x) + ", " + std::to_string(region_index.y) +
                          ") out of bounds.");

    return loadedRegions[region_index.x][region_index.y];
}