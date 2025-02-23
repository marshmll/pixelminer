#include "Map/TerrainGenerator.hxx"
#include "stdafx.hxx"

void TerrainGenerator::initPerlinWaves()
{
    logger.logInfo(_("Initializing perlin waves..."));
    msg = _("Initializing perlin waves...");

    heightWaves = {{120.f, .08f, 8.f}};
    moistureWaves = {{622.f, .06f, 6.f}, {344.f, .02f, 2.f}};
    heatWaves = {{318.6f, .06f, 8.f}};
}

void TerrainGenerator::initNoiseMaps()
{
    logger.logInfo(_("Generating noise maps..."));
    msg = _("Generating noise maps...");

    heightMap =
        perlinNoise.generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .06f, heightWaves, {0.f, 0.f});
    moistureMap =
        perlinNoise.generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .009f, moistureWaves, {10.f, 10.f});
    heatMap = perlinNoise.generateNoiseMap(MAX_WORLD_GRID_SIZE.x, MAX_WORLD_GRID_SIZE.y, .26f, heatWaves, {5.f, 5.f});
}

void TerrainGenerator::initBiomes()
{
    logger.logInfo(_("Precomputing biomes..."));
    msg = _("Precomputing biomes...");

    biomes = {
        {BiomeType::Desert, .2f, .1f, .85f},     {BiomeType::Forest, .4f, .85f, .8f},
        {BiomeType::Grassland, .55f, .5f, .35f}, {BiomeType::Jungle, .45f, .8f, .7f},
        {BiomeType::Mountains, .95f, .4f, .1f},  {BiomeType::Ocean, .15f, .5f, .6f},
        {BiomeType::Tundra, .65f, .4f, .1f},
    };

    // sf::Image mapImg(sf::Vector2u(MAX_WORLD_GRID_SIZE.x / 4, MAX_WORLD_GRID_SIZE.y / 4));

    for (int x = 0; x < MAX_WORLD_GRID_SIZE.x; ++x)
    {
        for (int y = 0; y < MAX_WORLD_GRID_SIZE.y; ++y)
        {
            float height = heightMap[x][y];
            float moisture = moistureMap[x][y];
            float heat = heatMap[x][y];

            float maxWeight = -1.f;
            for (const auto &biome : biomes)
            {
                float weight = biome.calculateWeight(height, moisture, heat);
                if (weight > maxWeight)
                {
                    maxWeight = weight;
                    biomeMap[x][y].name = biome.getName();
                    biomeMap[x][y].type = biome.getType();
                }
            }

            switch (biomeMap[x][y].type)
            {
            case BiomeType::Desert:
                biomeMap[x][y].color = sf::Color(194, 178, 128, 255);
                biomeMap[x][y].baseTileTag = "pixelminer:sand_tile";
                break;
            case BiomeType::Forest:
                biomeMap[x][y].color = sf::Color(24 * std::pow(3.f, (1.f - moisture + heat)), 110, 20, 255);
                biomeMap[x][y].baseTileTag = "pixelminer:grass_tile";
                break;
            case BiomeType::Grassland:
                biomeMap[x][y].color = sf::Color(26 * std::pow(3.2f, (1.f - moisture + heat)), 148, 24, 255);
                biomeMap[x][y].baseTileTag = "pixelminer:grass_tile";
                break;
            case BiomeType::Jungle:
                biomeMap[x][y].color = sf::Color(25 * std::pow(3.2f, (1.f - moisture + heat)), 130, 20, 255);
                biomeMap[x][y].baseTileTag = "pixelminer:grass_tile";
                break;
            case BiomeType::Mountains:
                biomeMap[x][y].color = sf::Color(150, 150, 150, 255);
                biomeMap[x][y].baseTileTag = "pixelminer:stone_tile";
                break;
            case BiomeType::Ocean:
                biomeMap[x][y].color = sf::Color(16, 51, 163, 255);
                biomeMap[x][y].baseTileTag = "pixelminer:water_tile";
                break;
            case BiomeType::Tundra:
                biomeMap[x][y].color = sf::Color(255, 255, 255, 255);
                biomeMap[x][y].baseTileTag = "pixelminer:snowy_grass_tile";
                break;
            default:
                biomeMap[x][y].color = sf::Color::White;
                biomeMap[x][y].baseTileTag = "unknown";
                break;
            }

            // mapImg.setPixel(sf::Vector2u(x / 4, y / 4), biomeMap[x][y].color);
        }
    }

    // if (!mapImg.saveToFile(GLOBAL_FOLDER + "map.png"))
    //     logger.logError(_("Failed to save map image."), false);
}

void TerrainGenerator::initRandomGrid()
{
    randomGrid.resize(MAX_WORLD_GRID_SIZE.x, std::vector<float>(MAX_WORLD_GRID_SIZE.y, 0.0f));

    for (unsigned int x = 0; x < MAX_WORLD_GRID_SIZE.x; ++x)
    {
        for (unsigned int y = 0; y < MAX_WORLD_GRID_SIZE.y; ++y)
        {
            // Generate a random value between 0 and 1 for each grid cell
            randomGrid[x][y] = rng.nextFloat();
        }
    }
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
        chunks[chunk_x][chunk_y] =
            std::make_unique<Chunk>(texturePack, sf::Vector2u(chunk_x, chunk_y), gridSize, scale, ChunkFlags::None);

    if (!chunks[chunk_x][chunk_y]->tiles[tile_x][tile_y][grid_z])
    {
        chunks[chunk_x][chunk_y]->tiles[tile_x][tile_y][grid_z] = std::make_unique<Tile>(tile);
        chunks[chunk_x][chunk_y]->updateVertexArray();
    }
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

TerrainGenerator::TerrainGenerator(std::string &msg, Metadata &metadata, ChunkMatrix &chunks, long int seed,
                                   sf::Texture &texture_pack, TileDatabase &tile_db, const unsigned int &grid_size,
                                   const float &scale)
    : logger("TerrainGenerator"), msg(msg), metadata(metadata), chunks(chunks), seed(seed), texturePack(texture_pack),
      tileDb(tile_db), gridSize(grid_size), scale(scale), rng(seed), perlinNoise(seed)
{
    using namespace std::chrono_literals;

    initPerlinWaves();
    initNoiseMaps();
    initBiomes();
    initRandomGrid();

    msg = _("Done!");
    std::this_thread::sleep_for(200ms);
}

TerrainGenerator::~TerrainGenerator() = default;

void TerrainGenerator::generateRegion(const sf::Vector2i &region_index)
{
    if (region_index.x > MAX_REGIONS.x - 1 || region_index.y > MAX_REGIONS.y - 1 || region_index.x < 0 ||
        region_index.y < 0)
    {
        logger.logError(_("Failed to generate region: Indexes are out of bounds."), false);
        return;
    }

    const int REGION_GRID_START_X = region_index.x * REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x;
    const int REGION_GRID_START_Y = region_index.y * REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y;
    const int REGION_GRID_END_X = (REGION_GRID_START_X + REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x) - 1;
    const int REGION_GRID_END_Y = (REGION_GRID_START_Y + REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y) - 1;

    for (int x = REGION_GRID_START_X; x <= REGION_GRID_END_X; ++x)
    {
        for (int y = REGION_GRID_START_Y; y <= REGION_GRID_END_Y; ++y)
        {
            const BiomePreset &biome = biomeMap[x][y];
            TileData tile_data = tileDb.getByTag(biome.baseTileTag);

            putTile(Tile(tile_data.name, tile_data.tag, tile_data.id, texturePack, tile_data.rect, gridSize, {}, scale,
                         biome.color),
                    x, y, 0);

            if (tile_data.tag == "pixelminer:grass_tile")
            {
                float randomValue = randomGrid[x][y];

                if (randomValue < 0.005f)
                {
                    TileData td = tileDb.getByTag("pixelminer:short_grass");
                    putTile(Tile(td.name, td.tag, td.id, texturePack, td.rect, gridSize, {}, scale, biome.color), x, y,
                            1);
                }
                if (randomValue < 0.002f)
                {
                    TileData td = tileDb.getByTag("pixelminer:arbust_1");
                    putTile(Tile(td.name, td.tag, td.id, texturePack, td.rect, gridSize, {}, scale, biome.color), x, y,
                            1);
                }
                if (randomValue < 0.001f)
                {
                    TileData td = tileDb.getByTag("pixelminer:arbust_2");
                    putTile(Tile(td.name, td.tag, td.id, texturePack, td.rect, gridSize, {}, scale, biome.color), x, y,
                            1);
                }
            }
            else if (tile_data.tag == "pixelminer:snowy_grass_tile")
            {
                float randomValue = randomGrid[x][y];

                if (randomValue < 0.01f)
                {
                    TileData td = tileDb.getByTag("pixelminer:snow_tile");
                    putTile(Tile(td.name, td.tag, td.id, texturePack, td.rect, gridSize, {}, scale, biome.color), x, y,
                            1);
                }
            }
        }
    }
}

const BiomePreset &TerrainGenerator::getBiomeData(const sf::Vector2u &grid_pos) const
{
    return biomeMap[grid_pos.x][grid_pos.y];
}

const float &TerrainGenerator::getHeightAt(const sf::Vector2u &grid_pos) const
{
    return heightMap[grid_pos.x][grid_pos.y];
}

const float &TerrainGenerator::getMoistureAt(const sf::Vector2u &grid_pos) const
{
    return moistureMap[grid_pos.x][grid_pos.y];
}

const float &TerrainGenerator::getHeatAt(const sf::Vector2u &grid_pos) const
{
    return heatMap[grid_pos.x][grid_pos.y];
}
