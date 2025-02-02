#pragma once

#include "Entities/Entity.hxx"

struct PlayerData : public EntityData
{
    float maxVelocity;
    uint8_t movFlags;
    uint8_t movDirection;
    uint8_t maxHealth;
    uint8_t health;
    uint8_t maxHunger;
    uint8_t hunger;
};

class Player : public Entity
{
  private:
    PlayerData playerData;

    void initPlayerAnimations();

    void preparePlayerData(const std::string &uuid);

    const bool loadPlayerData(const std::string &folder_name, const std::string &uuid);

  public:
    Player(const std::string &name, const std::string &folder_name, const std::string &uuid,
           const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale);

    ~Player();

    void update(const float &dt, const bool &update_movement = true);

    void render(sf::RenderTarget &target);

    void save(const std::string &folder_name, const std::string &uuid);
};