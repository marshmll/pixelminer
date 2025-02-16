/**
 * @file Biome.hxx
 * @brief Declares the Biome class to handle biome presets, types and functions.
 */

#pragma once

#include "Tiles/TileData.hxx"
#include "Engine/Languages.hxx"

/**
 * @enum BiomeType
 * @brief Enum representing different types of biomes.
 *
 * This enum defines a set of biome types that can be used to categorize areas of a map or world.
 */
enum BiomeType
{
    UnknownBiome = 0, ///< Represents an unknown or undefined biome.
    Desert,           ///< Desert biome type.
    Forest,           ///< Forest biome type.
    Grassland,        ///< Grassland biome type.
    Jungle,           ///< Jungle biome type.
    Mountains,        ///< Mountains biome type.
    Ocean,            ///< Ocean biome type.
    Tundra,           ///< Tundra biome type.
};

/**
 * @struct BiomePreset
 * @brief Struct representing the preset configuration for a biome.
 *
 * This struct holds preset information about a biome, such as its name, type, color, and associated tile ID.
 */
struct BiomePreset
{
    std::string name = "Unknown";             ///< Name of the biome.
    BiomeType type = BiomeType::UnknownBiome; ///< Type of the biome (from BiomeType enum).
    sf::Color color = sf::Color::White;       ///< Color representation of the biome.
    std::string baseTileId = "unknown";       ///< Base tile ID associated with the biome.
};

/**
 * @class Biome
 * @brief Class representing a biome with associated characteristics and behavior.
 *
 * This class provides information about a biome, including its type, ideal height, moisture, and heat
 * conditions. It also includes methods for calculating how closely a given set of environmental conditions
 * match the biome's ideal conditions.
 */
class Biome
{
  private:
    std::string name;    ///< Name of the biome.
    BiomeType type;      ///< Type of the biome (from BiomeType enum).
    float idealHeight;   ///< Ideal height for the biome.
    float idealMoisture; ///< Ideal moisture for the biome.
    float idealHeat;     ///< Ideal heat for the biome.

    /**
     * @brief Sets the name of the biome based on its type.
     *
     * This private helper function assigns a name to the biome based on its type from the BiomeType enum.
     */
    void setBiomeName();

  public:
    /**
     * @brief Constructs a Biome object with specified parameters.
     *
     * @param type The type of the biome (from BiomeType enum).
     * @param ideal_height The ideal height for the biome.
     * @param ideal_moisture The ideal moisture level for the biome.
     * @param ideal_heat The ideal heat level for the biome.
     */
    Biome(const BiomeType type, const float &ideal_height, const float &ideal_moisture, const float &ideal_heat);

    /**
     * @brief Destructor for the Biome class.
     */
    ~Biome();

    /**
     * @brief Calculates the weight of a given set of environmental conditions relative to the biome's ideal conditions.
     *
     * This function compares the height, moisture, and heat parameters against the biome's ideal values
     * and returns a weight representing how closely they match. The closer the values are to the ideal conditions,
     * the higher the weight.
     *
     * @param height The height to compare against the biome's ideal height.
     * @param moisture The moisture level to compare against the biome's ideal moisture.
     * @param heat The heat level to compare against the biome's ideal heat.
     * @return A floating-point value representing the weight, with higher values indicating a closer match.
     */
    const float calculateWeight(const float &height, const float &moisture, const float &heat) const;

    /**
     * @brief Gets the type of the biome.
     *
     * @return The type of the biome (from BiomeType enum).
     */
    const BiomeType &getType() const;

    /**
     * @brief Gets the name of the biome.
     *
     * @return The name of the biome as a string.
     */
    const std::string &getName() const;
};
