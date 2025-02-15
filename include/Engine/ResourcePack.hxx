/**
 * @file ResourcePack.hxx
 * @brief Declares the ResourcePack class for managing game assets.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Engine/Languages.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"
#include "Tools/Zip.hxx"
#include "Engine/Languages.hxx"

/**
 * @class ResourcePack
 * @brief Handles loading and managing game assets from resource packs.
 */
class ResourcePack
{
  private:
    Logger logger; ///< Logger instance for debugging and information logging.

  public:
    /**
     * @brief Constructs a ResourcePack instance.
     */
    ResourcePack();

    /**
     * @brief Default destructor for ResourcePack
     */
    ~ResourcePack();

    std::string name;        ///< Name of the resource pack.
    std::string description; ///< Description of the resource pack.
    sf::Texture icon;        ///< Icon representing the resource pack.

    std::unordered_map<std::string, sf::Font> fonts;               ///< Loaded fonts.
    std::unordered_map<std::string, sf::Texture> textures;         ///< Loaded textures.
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers; ///< Loaded sound buffers.
    std::unordered_map<std::string, TileData> tileDB;              ///< Tile data mappings.

    /**
     * @brief Loads the resource pack by name.
     * @param name Name of the resource pack to load.
     * @return True if the resource pack was successfully loaded, false otherwise.
     */
    const bool load(const std::string &name);

    /**
     * @brief Retrieves a texture from the resource pack.
     * @param key Key of the texture to retrieve.
     * @return Reference to the requested texture.
     */
    sf::Texture &getTexture(const std::string &key);
};
