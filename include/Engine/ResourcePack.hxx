/**
 * @file ResourcePack.hxx
 * @brief Declares the ResourcePack class for managing game assets.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Engine/Languages.hxx"
#include "Tiles/TileDatabase.hxx"
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

    std::unordered_map<std::string, sf::Font> fonts;                          ///< Loaded fonts.
    std::unordered_map<std::string, sf::Texture> textures;                    ///< Loaded textures.
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;            ///< Loaded sound buffers.
    std::unordered_map<std::string, std::shared_ptr<sf::Sound>> globalSounds; ///< Sounds that are preloaded by default.
    std::unordered_map<std::string, std::shared_ptr<sf::Music>> musics;       ///< Loaded musics.
    TileDatabase tileDb;                                                      ///< Tile database.

    /**
     * @brief Loads the resource pack by name.
     * @param name Name of the resource pack to load.
     * @return True if the resource pack was successfully loaded, false otherwise.
     */
    const bool load(const std::string &name);

    /**
     * @brief Deletes the resource pack cache files.
     */
    void deleteCache();

    /**
     * @brief Retrieves a font from the resource pack.
     * @param key Key of the font to retrieve.
     * @return Reference to the requested font.
     * @throws Exception if the font is not found.
     */
    sf::Font &getFont(const std::string &key);

    /**
     * @brief Retrieves a texture from the resource pack.
     * @param key Key of the texture to retrieve.
     * @return Reference to the requested texture.
     * @throws Exception if the texture is not found.
     */
    sf::Texture &getTexture(const std::string &key);

    /**
     * @brief Retrieves a sound buffer from the resource pack.
     * @param key Key of the sound buffer to retrieve.
     * @return Reference to the requested sound buffer.
     * @throws Exception if the sound buffer is not found.
     */
    sf::SoundBuffer &getSoundBuffer(const std::string &key);

    /**
     * @brief Retrieves a sound from the resource pack.
     * @param key Key of the sound to retrieve.
     * @return Reference to the requested sound.
     * @throws Exception if the sound is not found.
     */
    sf::Sound &getSound(const std::string &key);

    /**
     * @brief Retrieves a music from the resource pack.
     * @param key Key of the music to retrieve.
     * @return Reference to the requested music.
     * @throws Exception if the music is not found.
     */
    sf::Music &getMusic(const std::string &key);
};
