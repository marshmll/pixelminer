#include "Map/Map.hxx"
#include "stdafx.hxx"

void Map::clear()
{
    for (auto &x : chunks)
        x.clear();

    chunks.clear();
}

void Map::resize(const sf::Vector2<unsigned int> &amount_of_chunks)
{
    chunks.resize(amount_of_chunks.x);

    for (auto &vector : chunks)
        vector.resize(amount_of_chunks.y);

    dimensions.x = amount_of_chunks.x * CHUNK_SIZE.x;
    dimensions.y = amount_of_chunks.y * CHUNK_SIZE.y;
    dimensions.z = CHUNK_SIZE.z;
}

void Map::initPerlinWaves()
{
    height_waves = {{120.f, .009f, 4.f}, {300.f, .2f, 1.5f}, {500.f, .018f, 8.f}};
    moisture_waves = {{622.f, .04f, 5.f}, {200.f, .08f, 2.f}, {400.f, .2f, .8f}};
    heat_waves = {{318.6f, .05f, 5.f}, {329.7f, .5f, 1.f}};
}

void Map::initNoiseMaps()
{
    height_map = noise->generateNoiseMap(dimensions.x, dimensions.y, .12f, height_waves, {0.f, 0.f});
    moisture_map = noise->generateNoiseMap(dimensions.x, dimensions.y, .22f, moisture_waves, {10.f, 10.f});
    heat_map = noise->generateNoiseMap(dimensions.x, dimensions.y, .12f, heat_waves, {5.f, 5.f});
}

void Map::initBiomes()
{
    biomes = {
        {BiomeType::Desert, .3f, 0.1f, .9f},   {BiomeType::Forest, .4f, .6f, .4f},
        {BiomeType::Grassland, .3f, .5f, .5f}, {BiomeType::Jungle, .45f, .8f, .7f},
        {BiomeType::Mountains, .9f, .3f, .3f}, {BiomeType::Ocean, .35f, .7f, .4f},
        {BiomeType::Tundra, .8f, .3f, .1f},
    };
}

void Map::randomizeSpawnPoint()
{
    spawnPoint.x = static_cast<float>((std::rand() % dimensions.x) * gridSize);
    spawnPoint.y = static_cast<float>((std::rand() % dimensions.y) * gridSize);
}

Map::Map(const sf::Vector2<unsigned int> &amount_of_chunks, std::map<std::uint32_t, TileData> &tile_data,
         sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale, const long int &seed)
    : amountOfChunks(amount_of_chunks), tileData(tile_data), texturePack(texture_pack), gridSize(grid_size),
      scale(scale), seed(seed), noise(std::make_unique<PerlinNoise>(seed))
{
    clear();
    resize(amount_of_chunks);
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
    // for (auto &x : chunks)
    // {
    //     for (auto &chunk : x)
    //     {
    //         if (chunk)
    //             chunk->update(dt);
    //     }
    // }
}

void Map::render(sf::RenderTarget &target)
{
    // for (auto &x : chunks)
    // {
    //     for (auto &chunk : x)
    //     {
    //         if (chunk)
    //             chunk->render(target, false);
    //     }
    // }
}

void Map::putTile(TileBase tile, const unsigned int &x, const unsigned int &y, const unsigned int &z)
{
    unsigned int chunk_index_x = static_cast<unsigned int>(x / CHUNK_SIZE.x);
    unsigned int chunk_index_y = static_cast<unsigned int>(y / CHUNK_SIZE.y);

    if (chunk_index_x < amountOfChunks.x && chunk_index_y < amountOfChunks.y && z < CHUNK_SIZE.z)
    {
        unsigned int tile_chunk_x = x % CHUNK_SIZE.x;
        unsigned int tile_chunk_y = y % CHUNK_SIZE.x;

        // std::cout << "tile_chunk_x: " << tile_chunk_x << " tile_chunk_y: " << tile_chunk_y << "\n";

        tile.setGridPosition(
            sf::Vector2u(tile_chunk_x + (chunk_index_x * CHUNK_SIZE.x), tile_chunk_y + (chunk_index_y * CHUNK_SIZE.y)));

        if (chunks[chunk_index_x][chunk_index_y])
        {
            chunks[chunk_index_x][chunk_index_y]->tiles[tile_chunk_x][tile_chunk_y][z] =
                std::make_unique<TileBase>(tile);
        }
        else
        {
            chunks[chunk_index_x][chunk_index_y] =
                std::make_unique<Chunk>(sf::Vector2u(chunk_index_x, chunk_index_y), gridSize, scale);

            chunks[chunk_index_x][chunk_index_y]->tiles[tile_chunk_x][tile_chunk_y][z] =
                std::make_unique<TileBase>(tile);
        }
    }
}

void Map::saveToFile(std::filesystem::path path)
{
    std::ofstream out(path);

    if (!out.is_open())
        throw std::runtime_error("[ Map::saveToFile ] -> ERROR: Unable to write to file: " + path.string() + "\n");

    /* MAP HEADER */

    // Creation data timestamp

    // Chunks Amount X, Chunks Amount Y
    out << amountOfChunks.x << " " << amountOfChunks.y << "\n";

    // Spawnpoint X, y
    out << spawnPoint.x << " " << spawnPoint.y << "\n";

    /* MAP DATA */
    for (auto &x : chunks)
    {
        for (auto &chunk : x)
        {
            if (chunk)
            {
                out << chunk->chunkIndex.x << " " << chunk->chunkIndex.y << "\n";

                for (unsigned int x = 0; x < CHUNK_SIZE.x; x++)
                {
                    for (unsigned int y = 0; y < CHUNK_SIZE.y; y++)
                    {
                        for (unsigned int z = 0; z < CHUNK_SIZE.z; z++)
                        {
                            if (chunk->tiles[x][y][z])
                            {
                                out << x << " " << y << " " << z << " " << chunk->tiles[x][y][z]->getId() << " ";

                                if (chunk->tiles[x][y][z]->getId() == TileId::GrassTop)
                                    out << static_cast<unsigned>(chunk->tiles[x][y][z]->getColor().r) << " "
                                        << static_cast<unsigned>(chunk->tiles[x][y][z]->getColor().g) << " "
                                        << static_cast<unsigned>(chunk->tiles[x][y][z]->getColor().b) << " "
                                        << static_cast<unsigned>(chunk->tiles[x][y][z]->getColor().a) << " ";

                                out << "\n";
                            }
                            else
                            {
                                out << "0 0 0 0" << "\n";
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << "[ Map::saveToFile ] -> Map saved to: " << path.string() << "\n";

    out.close();
}

void Map::loadFromFile(std::filesystem::path path)
{
    std::ifstream in(path);

    if (!in.is_open())
        throw std::runtime_error("[ Map::loadFromFile ] -> ERROR: Unable to load from file: " + path.string() + "\n");

    /* MAP HEADER */

    // Creation data timestamp

    // Chunks Amount X, Chunks Amount Y
    in >> amountOfChunks.x >> amountOfChunks.y;
    resize(amountOfChunks);

    // Spawnpoint X, Y
    in >> spawnPoint.x >> spawnPoint.y;

    unsigned int chunk_index_x;
    unsigned int chunk_index_y;

    /* MAP DATA */
    while (in >> chunk_index_x >> chunk_index_y)
    {
        chunks[chunk_index_x][chunk_index_y] =
            std::make_unique<Chunk>(sf::Vector2u(chunk_index_x, chunk_index_y), gridSize, scale);
        unsigned int x, y, z;
        std::int32_t tile_id;
        sf::Color tile_color;

        for (unsigned int i = 0; i < (CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z); i++)
        {
            in >> x >> y >> z >> tile_id; // x, y and z relative to chunk

            if (x + y + z + tile_id == 0)
                continue;

            TileData t_data = tileData.at(tile_id);

            chunks[chunk_index_x][chunk_index_y]->tiles[x][y][z] = std::make_unique<Tile>(
                t_data.name, t_data.id, texturePack, t_data.textureRect, gridSize,
                sf::Vector2u(x + (chunk_index_x * CHUNK_SIZE.x), y + (chunk_index_y * CHUNK_SIZE.y)), scale);

            if (tile_id == TileId::GrassTop)
            {
                in >> tile_color.r >> tile_color.g >> tile_color.b >> tile_color.a;

                chunks[chunk_index_x][chunk_index_y]->tiles[x][y][z]->setColor(tile_color);
            }
        }
    }

    std::cout << "[ Map::saveToFile ] -> Map loaded from: " << path.string() << "\n";

    in.close();
}

const sf::Vector2f &Map::getSpawnPoint() const
{
    return spawnPoint;
}