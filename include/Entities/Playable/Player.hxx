/**
 * @file Player.hxx
 * @brief Declares the Player class for managing playable characthers.
 */

#pragma once

#include "Entities/Entity.hxx"

/**
 * @struct PlayerData
 * @brief Extends EntityData to include player-specific attributes like health, hunger, and movement data.
 */
struct PlayerData : public EntityData
{
    float maxVelocity;    ///< Maximum velocity of the player.
    uint8_t movFlags;     ///< Bitmask flags for allowed movements.
    uint8_t movDirection; ///< Current movement direction of the player.
    uint8_t maxHealth;    ///< Maximum health of the player.
    uint8_t health;       ///< Current health of the player.
    uint8_t maxHunger;    ///< Maximum hunger of the player.
    uint8_t hunger;       ///< Current hunger of the player.
};

/**
 * @class Player
 * @brief Represents a playable character in the game. Inherits from Entity and adds player-specific functionality.
 */
class Player : public Entity
{
  private:
    PlayerData playerData; ///< Stores player-specific data.

    sf::Clock walkSoundClock;
    uint32_t walkTimeMax;

    /**
     * @brief Initializes animations for the player.
     */
    void initAnimations();

    /**
     * @brief Initializes hitboxes for the player.
     */
    void initHitBoxes();

    /**
     * @brief Initializes sounds for the player.
     */
    void initSounds();

    /**
     * @brief Prepares player data for saving or loading.
     * @param uuid Unique identifier for the player.
     */
    void preparePlayerData(const std::string &uuid);

    /**
     * @brief Loads player data from a file.
     * @param folder_name Name of the folder containing the player data.
     * @param uuid Unique identifier for the player.
     * @return True if the data was successfully loaded, false otherwise.
     */
    const bool loadPlayerData(const std::string &folder_name, const std::string &uuid);

  public:
    /**
     * @brief Constructs a Player object.
     * @param name Name of the player.
     * @param folder_name Name of the folder containing the player data.
     * @param uuid Unique identifier for the player.
     * @param spawn_grid_position Spawn position of the player in grid coordinates.
     * @param sprite_sheet Reference to the sprite sheet texture.
     * @param scale Scaling factor for the player's sprites.
     */
    Player(const std::string &name, const std::string &folder_name, const std::string &uuid,
           const sf::Vector2f &spawn_grid_position, sf::Texture &sprite_sheet, const float &scale,
           std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers);

    /**
     * @brief Destructor for the Player class.
     */
    ~Player();

    /**
     * @brief Updates the player's state.
     * @param dt Delta time for frame-independent updates.
     * @param mouse_pos Current position of the mouse.
     */
    void update(const float &dt, const sf::Vector2f &mouse_pos) override;

    /**
     * @brief Renders the player on the target.
     * @param target Render target to draw the player on.
     */
    void render(sf::RenderTarget &target) override;

    /**
     * @brief Updates the player's state, optionally updating movement.
     * @param dt Delta time for frame-independent updates.
     * @param update_movement Whether to update movement.
     * @param tile_name_under The name of the tile that the player is stepping on.
     */
    void update(const float &dt, const bool &update_movement, const std::string &tile_name_under);

    /**
     * @brief Renders the player on the target, optionally showing hitboxes.
     * @param target Render target to draw the player on.
     * @param show_hitboxes Whether to render hitboxes.
     */
    void render(sf::RenderTarget &target, const bool &show_hitboxes) override;

    /**
     * @brief Saves the player's data to a file.
     * @param folder_name Name of the folder to save the player data.
     * @param uuid Unique identifier for the player.
     */
    void save(const std::string &folder_name, const std::string &uuid);
};
