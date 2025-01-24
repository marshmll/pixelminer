#include "Map/Map.hxx"
#include "stdafx.hxx"

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
    metadata.spawnX = static_cast<float>((std::rand() % MAX_WORLD_GRID_SIZE.x) * gridSize);
    metadata.spawnY = static_cast<float>((std::rand() % MAX_WORLD_GRID_SIZE.y) * gridSize);
    metadata.timePlayed = 0;
}

void Map::initTerrainGenerator(const long int &seed)
{
    terrainGenerator = std::make_unique<TerrainGenerator>(chunks, seed, texturePack, tileData, gridSize, scale);
}

Map::Map(const std::string &name, const long int &seed, std::map<std::uint32_t, TileData> &tile_data,
         sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale)

    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale), rng(seed)
{
    initMetadata(name, seed);
    initTerrainGenerator(seed);
    loadRegion({0, 0});
    loadRegion({0, 1});
}

Map::Map(std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack, const unsigned int &grid_size,
         const float &scale)
    : tileData(tile_data), texturePack(texture_pack), gridSize(grid_size), scale(scale), rng(0)
{
}

Map::~Map()
{
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

                    std::cout << chunk_x << " " << chunk_y << "\n";

                    region_file.write(reinterpret_cast<char *>(&chunk_x), sizeof(unsigned short));
                    region_file.write(reinterpret_cast<char *>(&chunk_y), sizeof(unsigned short));
                    region_file.write(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned int));
                    region_file.write(reinterpret_cast<char *>(&chunk->flags), sizeof(uint8_t));

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
            region_file.close();

            if (std::filesystem::file_size(path_str + "regions/" + fname) == 0)
                std::remove(std::string(path_str + "regions/" + fname).c_str());
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

    /* TERRAIN GENERATOR ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    // Reconfigure terraing generator.
    if (terrainGenerator.get())
    {
        terrainGenerator.reset();
        initTerrainGenerator(metadata.seed);
    }

    /* REGIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    if (!std::filesystem::exists(path_str + "regions/"))
        throw std::runtime_error("[ Map::loadFromFile ] -> Invalid world: " + path_str + "\n");

    std::cout << "[ Map::loadFromFile ] -> Map loaded from: " << path_str << "\n";
}

void Map::loadRegion(const sf::Vector2u &region_index)
{
    std::string filename = "r." + std::to_string(region_index.x) + "." + std::to_string(region_index.y) + ".region";
    std::stringstream path_sstream;
    path_sstream << MAPS_FOLDER + metadata.name << "/regions/" << filename;

    // std::cout << path_sstream.str() << "\n";

    if (!std::filesystem::exists(path_sstream.str()))
    {
        terrainGenerator->generateRegion(region_index);
        std::cout << "Generating region " << region_index.x << " " << region_index.y << "\n";
        return;
    }

    std::ifstream region_file(path_sstream.str(), std::ios::binary);

    if (!region_file.is_open())
        throw std::runtime_error("Failed to open region file: " + filename);

    unsigned short chunk_x = 0, chunk_y = 0;
    unsigned int tile_amount = 0;
    uint8_t flags = 0;

    if (!region_file.read(reinterpret_cast<char *>(&chunk_x), sizeof(unsigned short)) ||
        !region_file.read(reinterpret_cast<char *>(&chunk_y), sizeof(unsigned short)) ||
        !region_file.read(reinterpret_cast<char *>(&tile_amount), sizeof(unsigned int)) ||
        !region_file.read(reinterpret_cast<char *>(&flags), sizeof(uint8_t)))
    {
        throw std::runtime_error("Region file seems to be corrupted: " + filename);
    }

    // std::cout << chunk_x << " " << chunk_y << "\n";

    chunks[chunk_x][chunk_y] = std::make_unique<Chunk>(sf::Vector2u(chunk_x, chunk_y), gridSize, scale, flags);

    Chunk *new_chunk = chunks[chunk_x][chunk_y].get();

    // std::cout << tile_amount << "\n";

    for (int l = 0; l < tile_amount; l++)
    {
        unsigned short x = 0, y = 0, z = 0;
        std::uint32_t id;

        region_file.read(reinterpret_cast<char *>(&x), sizeof(unsigned short));
        region_file.read(reinterpret_cast<char *>(&y), sizeof(unsigned short));
        region_file.read(reinterpret_cast<char *>(&z), sizeof(unsigned short));
        region_file.read(reinterpret_cast<char *>(&id), sizeof(std::uint32_t));

        // std::cout << x << " " << y << " " << z << "\n";

        TileData data;
        sf::Vector2u grid_pos =
            sf::Vector2u(x + (chunk_x * CHUNK_SIZE_IN_TILES.x), y + (chunk_y * CHUNK_SIZE_IN_TILES.y));

        if (tileData.count(id) == 0)
            data = tileData.at(TileId::Unknown);
        else
            data = tileData.at(id);

        new_chunk->tiles[x][y][z] =
            std::make_unique<Tile>(data.name, data.id, texturePack, data.textureRect, gridSize, grid_pos, scale);

        if (id == TileId::GrassTile)
        {
            sf::Color color;
            region_file.read(reinterpret_cast<char *>(&color.r), sizeof(std::uint8_t));
            region_file.read(reinterpret_cast<char *>(&color.g), sizeof(std::uint8_t));
            region_file.read(reinterpret_cast<char *>(&color.b), sizeof(std::uint8_t));

            new_chunk->tiles[x][y][z]->setColor(color);
        }
    }

    region_file.close();
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
