#include "Entities/Playable/Player.hxx"
#include "stdafx.hxx"

void Player::initPlayerAnimations()
{
    sf::Vector2u frame_size(16, 24);

    animationFunctionality->addAnimation("IdleDown", 10000, frame_size, {0, 0}, {0, 0});
    animationFunctionality->addAnimation("IdleUp", 10000, frame_size, {0, 1}, {0, 1});
    animationFunctionality->addAnimation("IdleLeft", 10000, frame_size, {0, 2}, {0, 2});
    animationFunctionality->addAnimation("IdleRight", 10000, frame_size, {0, 3}, {0, 3});
    animationFunctionality->addAnimation("WalkDown", 150, frame_size, {0, 0}, {3, 0});
    animationFunctionality->addAnimation("WalkUp", 150, frame_size, {0, 1}, {3, 1});
    animationFunctionality->addAnimation("WalkLeft", 150, frame_size, {0, 2}, {3, 2});
    animationFunctionality->addAnimation("WalkRight", 150, frame_size, {0, 3}, {3, 3});
}

void Player::preparePlayerData(const std::string &uuid)
{
    playerData.name = name;
    playerData.currentPosition = getPosition();
    playerData.spawnPosition = spawnPosition;
    playerData.maxVelocity = movementFunctionality->getMaxVelocity();
    playerData.movFlags = movementFunctionality->getFlags();
    playerData.movDirection = movementFunctionality->getDirection();
    playerData.maxHealth = attributeFunctionality->getMaxHealth();
    playerData.health = attributeFunctionality->getHealth();
    playerData.maxHunger = attributeFunctionality->getMaxHunger();
    playerData.hunger = attributeFunctionality->getHunger();
}

const bool Player::loadPlayerData(const std::string &folder_name, const std::string &uuid)
{
    if (!std::filesystem::exists(MAPS_FOLDER + folder_name + "/players/" + uuid + ".dat"))
    {
        std::cerr << "[ Player ] -> Failed to load player data: no save file was found." << "\n";
        return false;
    }

    std::ifstream player_data_file(MAPS_FOLDER + folder_name + "/players/" + uuid + ".dat");

    if (!player_data_file.is_open())
        throw std::runtime_error("[ Player ] -> Failed to open player data file: " + uuid + ".dat");

    uint8_t name_size;
    char buf[256];

    // Name
    player_data_file.read(reinterpret_cast<char *>(&name_size), sizeof(uint8_t));
    player_data_file.read(buf, name_size);
    playerData.name = std::string(buf, name_size);

    // Current Position
    player_data_file.read(reinterpret_cast<char *>(&playerData.currentPosition.x), sizeof(float));
    player_data_file.read(reinterpret_cast<char *>(&playerData.currentPosition.y), sizeof(float));

    // Spawnpoint
    player_data_file.read(reinterpret_cast<char *>(&playerData.spawnPosition.x), sizeof(float));
    player_data_file.read(reinterpret_cast<char *>(&playerData.spawnPosition.y), sizeof(float));
    spawnPosition = playerData.spawnPosition;

    // Movement
    player_data_file.read(reinterpret_cast<char *>(&playerData.maxVelocity), sizeof(float));
    player_data_file.read(reinterpret_cast<char *>(&playerData.movFlags), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.movDirection), sizeof(uint8_t));

    // Attributes
    player_data_file.read(reinterpret_cast<char *>(&playerData.maxHealth), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.health), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.maxHunger), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.hunger), sizeof(uint8_t));

    std::cout << playerData.name << "\n"
              << playerData.currentPosition.x << ", " << playerData.currentPosition.y << "\n"
              << playerData.spawnPosition.x << ", " << playerData.spawnPosition.y << "\n"
              << playerData.maxVelocity << "\n"
              << static_cast<int>(playerData.movFlags) << "\n"
              << static_cast<int>(playerData.movDirection) << "\n";

    player_data_file.close();
    std::cout << "[ Player ] -> PlayerData was read from world save: " << folder_name << "\n";
    return true;
}

Player::Player(const std::string &name, const std::string &uuid, const sf::Vector2f spawn_position,
               sf::Texture &sprite_sheet, const float &scale)
    : Entity(name, spawn_position, sprite_sheet, scale)
{
    createMovementFunctionality(100.f, Movement::AllowAll);
    createAnimationFunctionality();
    createAttributeFunctionality(20, 20);
    initPlayerAnimations();

    std::cout << "[ Player ] -> Player \"" << name << "\" with id " << std::hex << id << " spawned at x: " << std::dec
              << spawn_position.x << ", y: " << spawn_position.y << "\n";
}

Player::Player(const std::string &folder_name, const std::string &uuid, sf::Texture &sprite_sheet, const float &scale)
    : Entity("", sf::Vector2f(), sprite_sheet, scale)
{
    if (!loadPlayerData(folder_name, uuid))
        throw std::runtime_error("Could not load player data to player with UUID: " + uuid);

    name = playerData.name;
    spawnPosition = playerData.spawnPosition;
    setPosition(playerData.currentPosition);
    createMovementFunctionality(playerData.maxVelocity, playerData.movFlags, playerData.movDirection);
    createAnimationFunctionality();
    createAttributeFunctionality(playerData.maxHealth, playerData.maxHunger);
    attributeFunctionality->setHealth(playerData.health);
    attributeFunctionality->setHunger(playerData.hunger);
    initPlayerAnimations();

    std::cout << "[ Player ] -> Player \"" << name << "\" with id " << std::hex << id
              << " loaded from file, spawned at x: " << std::dec << getPosition().x << ", y: " << getPosition().y
              << "\n";
}

Player::~Player()
{
}

void Player::update(const float &dt, const bool &update_movement)
{
    if (update_movement)
    {
        movementFunctionality->update();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            move(dt, Up);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            move(dt, Down);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            move(dt, Left);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            move(dt, Right);
    }

    const uint8_t mov_state = movementFunctionality->getState();
    const std::string mov_direction = movementFunctionality->getDirectionAsString();

    if (mov_state == MovementState::Idle)
        animationFunctionality->play("Idle" + mov_direction, true);

    else if (mov_state == MovementState::Walking)
        animationFunctionality->play("Walk" + mov_direction, true);
}

void Player::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}

void Player::save(const std::string &folder_name, const std::string &uuid)
{
    const std::filesystem::path root = MAPS_FOLDER + folder_name;

    if (!std::filesystem::exists(root))
        throw std::runtime_error("[ Player ] -> Failed to save " + name + " to map " + folder_name +
                                 ": Folder does not exist.");

    if (!std::filesystem::exists(root / "players/"))
        std::filesystem::create_directory(root / "players/");

    preparePlayerData(uuid);

    std::ofstream player_data_file(root / "players" / (uuid + ".dat"));

    if (!player_data_file.is_open())
        throw std::runtime_error("[ Player ] -> Failed to save player data: " + name);

    uint8_t name_size = playerData.name.size();

    player_data_file.write(reinterpret_cast<char *>(&name_size), sizeof(uint8_t));
    player_data_file.write(reinterpret_cast<char *>(playerData.name.data()), playerData.name.size());

    player_data_file.write(reinterpret_cast<char *>(&playerData.currentPosition.x), sizeof(float));
    player_data_file.write(reinterpret_cast<char *>(&playerData.currentPosition.y), sizeof(float));

    player_data_file.write(reinterpret_cast<char *>(&playerData.spawnPosition.x), sizeof(float));
    player_data_file.write(reinterpret_cast<char *>(&playerData.spawnPosition.y), sizeof(float));

    player_data_file.write(reinterpret_cast<char *>(&playerData.maxVelocity), sizeof(float));
    player_data_file.write(reinterpret_cast<char *>(&playerData.movFlags), sizeof(uint8_t));
    player_data_file.write(reinterpret_cast<char *>(&playerData.movDirection), sizeof(uint8_t));

    player_data_file.write(reinterpret_cast<char *>(&playerData.maxHealth), sizeof(uint8_t));
    player_data_file.write(reinterpret_cast<char *>(&playerData.health), sizeof(uint8_t));
    player_data_file.write(reinterpret_cast<char *>(&playerData.maxHunger), sizeof(uint8_t));
    player_data_file.write(reinterpret_cast<char *>(&playerData.hunger), sizeof(uint8_t));

    if (player_data_file.bad())
    {
        player_data_file.close();
        throw std::runtime_error("[ Player ] -> Something went wrong while saving player data: " + name +
                                 " (bad bit was set).");
    }

    player_data_file.close();

    std::cout << "[ Player ] -> Player data saved to player: " << name << "\n";
}