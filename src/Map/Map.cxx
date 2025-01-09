#include "Map/Map.hxx"
#include "stdafx.hxx"

void Map::clear()
{
    for (auto &x : chunks)
    {
        x.clear();
    }

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

Map::Map(const sf::Vector2<unsigned int> &amount_of_chunks, std::map<std::uint32_t, TileData> &tile_data,
         sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale)
    : amountOfChunks(amount_of_chunks), tileData(tile_data), texturePack(texture_pack), gridSize(grid_size),
      scale(scale)
{
    clear();
    resize(amount_of_chunks);
    initPerlinWaves();
    initNoiseMaps();
    initBiomes();
    generate();

    // loadFromFile("Assets/Maps/mymap.map");
    // saveToFile("Assets/Maps/mymap.map");
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
                biome_color = sf::Color(196, 178, 94, 255);
                tile_data = tileData.at(TileId::Cobblestone);

                break;
            case Forest:
                biome_color = sf::Color(29, 110, 29, 255);
                tile_data = tileData.at(TileId::Dirt);
                break;
            case Grassland:
                biome_color = sf::Color(22, 148, 22, 255);
                tile_data = tileData.at(TileId::GrassTop);
                break;
            case Jungle:
                biome_color = sf::Color(2, 31, 8, 255);
                tile_data = tileData.at(TileId::GrassSide);
                break;
            case Mountains:
                biome_color = sf::Color(91, 99, 93, 255);
                tile_data = tileData.at(TileId::Stone);
                break;
            case Ocean:
                biome_color = sf::Color(16, 51, 163);
                tile_data = tileData.at(TileId::Cobblestone);
                break;
            case Tundra:
                biome_color = sf::Color(216, 242, 230, 255);
                tile_data = tileData.at(TileId::Stone);
                break;
            };

            image.setPixel({x, y}, biome_color);
            putTile(Tile(tile_data.name, tile_data.id, texturePack, tile_data.textureRect, gridSize, {}, scale), x, y,
                    0);
        }
    }

    (void)image.saveToFile("Assets/map.png");
}

void Map::update(const float &dt)
{
    for (auto &x : chunks)
    {
        for (auto &chunk : x)
        {
            if (chunk)
                chunk->update(dt);
        }
    }
}

void Map::render(sf::RenderTarget &target)
{
    for (auto &x : chunks)
    {
        for (auto &chunk : x)
        {
            if (chunk)
                chunk->render(target, true);
        }
    }
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
                                out << x << " " << y << " " << z << " " << chunk->tiles[x][y][z]->getId() << "\n";
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

    unsigned int chunk_index_x;
    unsigned int chunk_index_y;

    /* MAP DATA */
    while (in >> chunk_index_x >> chunk_index_y)
    {
        chunks[chunk_index_x][chunk_index_y] =
            std::make_unique<Chunk>(sf::Vector2u(chunk_index_x, chunk_index_y), gridSize, scale);
        unsigned int x, y, z;
        std::int32_t tile_id;

        for (unsigned int i = 0; i < (CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z); i++)
        {
            in >> x >> y >> z >> tile_id; // x, y and z relative to chunk

            if (x + y + z + tile_id == 0)
                continue;

            TileData t_data = tileData.at(tile_id);

            chunks[chunk_index_x][chunk_index_y]->tiles[x][y][z] = std::make_unique<Tile>(
                t_data.name, t_data.id, texturePack, t_data.textureRect, gridSize,
                sf::Vector2u(x + (chunk_index_x * CHUNK_SIZE.x), y + (chunk_index_y * CHUNK_SIZE.y)), scale);
        }
    }

    std::cout << "[ Map::saveToFile ] -> Map loaded from: " << path.string() << "\n";

    in.close();
}