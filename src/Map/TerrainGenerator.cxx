#include "Map/TerrainGenerator.hxx"
#include "stdafx.hxx"

void TerrainGenerator::initPerlinWaves()
{
    heightWaves = {{120.f, .009f, 4.f}, {300.f, .2f, 1.5f}, {500.f, .018f, 8.f}};
    moistureWaves = {{622.f, .04f, 5.f}, {200.f, .08f, 2.f}, {400.f, .2f, .8f}};
    heatWaves = {{318.6f, .05f, 5.f}, {329.7f, .5f, 1.f}};
}

void TerrainGenerator::initNoiseMaps()
{
    heightMap =
        perlinNoise.generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .08f, heightWaves, {0.f, 0.f});
    moistureMap =
        perlinNoise.generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .18f, moistureWaves, {10.f, 10.f});
    heatMap = perlinNoise.generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .08f, heatWaves, {5.f, 5.f});
}

void TerrainGenerator::initBiomes()
{
    biomes = {
        {BiomeType::Desert, .35f, 0.1f, .8f},  {BiomeType::Forest, .4f, .6f, .4f},
        {BiomeType::Grassland, .3f, .5f, .5f}, {BiomeType::Jungle, .45f, .8f, .7f},
        {BiomeType::Mountains, .9f, .3f, .3f}, {BiomeType::Ocean, .35f, .7f, .4f},
        {BiomeType::Tundra, .8f, .3f, .1f},
    };
}

void TerrainGenerator::putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z)
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

Tile *TerrainGenerator::getTile(const int &grid_x, const int &grid_y, const int &grid_z)
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

TerrainGenerator::TerrainGenerator(ChunkMatrix &chunks, const long int seed, sf::Texture &texture_pack,
                                   std::map<std::uint32_t, TileData> &tile_data, const unsigned int &grid_size,
                                   const float &scale)
    : logger("TerrainGenerator"), chunks(chunks), seed(seed), texturePack(texture_pack), tileData(tile_data),
      gridSize(grid_size), scale(scale), rng(seed), perlinNoise(seed)
{
    initPerlinWaves();
    initNoiseMaps();
    initBiomes();
}

TerrainGenerator::~TerrainGenerator()
{
}

void TerrainGenerator::generateRegion(const sf::Vector2u &region_index)
{
    if (region_index.x > MAX_REGIONS.x - 1 || region_index.y > MAX_REGIONS.y - 1 || region_index.x < 0 ||
        region_index.y < 0)
    {
        logger.logError("Failed to generate region: Indexes are out of bounds.", false);
        return;
    }

    Random rng(seed); // Same RNG for all generations.
    sf::Image image({MAX_WORLD_GRID_SIZE.x / 2, MAX_WORLD_GRID_SIZE.y / 2});

    const int REGION_GRID_START_X = region_index.x * REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x;
    const int REGION_GRID_START_Y = region_index.y * REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y;
    const int REGION_GRID_END_X = (REGION_GRID_START_X + REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x) - 1;
    const int REGION_GRID_END_Y = (REGION_GRID_START_Y + REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y) - 1;

    // Smooth biome transitions
    for (unsigned int x = 0; x < MAX_WORLD_GRID_SIZE.x; x++)
    {
        for (unsigned int y = 0; y < MAX_WORLD_GRID_SIZE.y; y++)
        {
            // Skip positions that should not be generated.
            if (x < REGION_GRID_START_X || y < REGION_GRID_START_Y || x > REGION_GRID_END_X || y > REGION_GRID_END_Y)
                continue;

            float height = heightMap[x][y];
            float moisture = moistureMap[x][y];
            float heat = heatMap[x][y];

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
}