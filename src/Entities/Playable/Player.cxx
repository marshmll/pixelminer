#include "Entities/Playable/Player.hxx"
#include "stdafx.hxx"

void Player::initAnimations()
{
    sf::Vector2u frame_size(16, 24);

    animationFunctionality->addAnimation("Base", "IdleDown", 10000, frame_size, {0, 0}, {0, 0});
    animationFunctionality->addAnimation("Base", "IdleUp", 10000, frame_size, {0, 1}, {0, 1});
    animationFunctionality->addAnimation("Base", "IdleLeft", 10000, frame_size, {0, 2}, {0, 2});
    animationFunctionality->addAnimation("Base", "IdleRight", 10000, frame_size, {0, 3}, {0, 3});
    animationFunctionality->addAnimation("Base", "WalkDown", 170, frame_size, {0, 0}, {3, 0});
    animationFunctionality->addAnimation("Base", "WalkUp", 170, frame_size, {0, 1}, {3, 1});
    animationFunctionality->addAnimation("Base", "WalkLeft", 170, frame_size, {0, 2}, {3, 2});
    animationFunctionality->addAnimation("Base", "WalkRight", 170, frame_size, {0, 3}, {3, 3});
}

void Player::initHitBoxes()
{
    collisionFunctionality->addHitBox("Body", sf::Vector2u(8, 2), sf::Vector2u(4, 20), scale);
}

void Player::initSounds()
{
    walkSoundClock.restart();
    walkTimeMax = 360;

    soundFunctionality->addSound("GrassWalk1", "GrassWalk1", 70.f);
    soundFunctionality->addSound("GrassWalk2", "GrassWalk2", 70.f);
    soundFunctionality->addSound("GrassWalk3", "GrassWalk3", 70.f);
    soundFunctionality->addSound("GrassWalk4", "GrassWalk4", 70.f);
    soundFunctionality->addSound("GrassWalk5", "GrassWalk5", 70.f);

    soundFunctionality->addSound("StoneWalk1", "StoneWalk1", 100.f);
    soundFunctionality->addSound("StoneWalk2", "StoneWalk2", 100.f);
    soundFunctionality->addSound("StoneWalk3", "StoneWalk3", 100.f);
    soundFunctionality->addSound("StoneWalk4", "StoneWalk4", 100.f);
    soundFunctionality->addSound("StoneWalk5", "StoneWalk5", 100.f);
}

void Player::preparePlayerData(const std::string &uuid)
{
    playerData.name = name;
    playerData.currentGridPosition = getGridPosition();
    playerData.spawnGridPosition = spawnGridPosition;
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
        logger.logWarning("Failed to load player data: no save file was found.");
        return false;
    }

    std::ifstream player_data_file(MAPS_FOLDER + folder_name + "/players/" + uuid + ".dat");

    if (!player_data_file.is_open())
        logger.logError("Failed to open player data file: " + uuid + ".dat");

    // Current Position
    player_data_file.read(reinterpret_cast<char *>(&playerData.currentGridPosition.x), sizeof(float));
    player_data_file.read(reinterpret_cast<char *>(&playerData.currentGridPosition.y), sizeof(float));

    // Spawnpoint
    player_data_file.read(reinterpret_cast<char *>(&playerData.spawnGridPosition.x), sizeof(float));
    player_data_file.read(reinterpret_cast<char *>(&playerData.spawnGridPosition.y), sizeof(float));
    spawnGridPosition = playerData.spawnGridPosition;

    // Movement
    player_data_file.read(reinterpret_cast<char *>(&playerData.maxVelocity), sizeof(float));
    player_data_file.read(reinterpret_cast<char *>(&playerData.movFlags), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.movDirection), sizeof(uint8_t));

    // Attributes
    player_data_file.read(reinterpret_cast<char *>(&playerData.maxHealth), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.health), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.maxHunger), sizeof(uint8_t));
    player_data_file.read(reinterpret_cast<char *>(&playerData.hunger), sizeof(uint8_t));

    // std::cout << playerData.currentGridPosition.x << ", " << playerData.currentGridPosition.y << "\n"
    //           << playerData.spawnGridPosition.x << ", " << playerData.spawnGridPosition.y << "\n"
    //           << playerData.maxVelocity << "\n"
    //           << static_cast<int>(playerData.movFlags) << "\n"
    //           << static_cast<int>(playerData.movDirection) << "\n";

    player_data_file.close();
    logger.logInfo("Player data was read from world save: " + folder_name);
    return true;
}

Player::Player(const std::string &name, const std::string &folder_name, const std::string &uuid,
               const sf::Vector2f &spawn_grid_position, sf::Texture &sprite_sheet, const float &scale,
               std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers)
    : Entity(name, EntityType::PlayerEntity, spawn_grid_position, sprite_sheet, scale, sound_buffers,
             RenderBehavior::Perspective)
{
    createAnimationFunctionality();
    createCollisionFunctionality();
    createSoundFunctionality();

    initAnimations();
    initHitBoxes();
    initSounds();

    if (loadPlayerData(folder_name, uuid))
    {
        spawnGridPosition = playerData.spawnGridPosition;
        setGridPosition(playerData.currentGridPosition);
        createMovementFunctionality(playerData.maxVelocity, playerData.movFlags, playerData.movDirection);
        createAttributeFunctionality(playerData.maxHealth, playerData.maxHunger);
        attributeFunctionality->setHealth(playerData.health);
        attributeFunctionality->setHunger(playerData.hunger);

        logger.logInfo(_("Player \"") + name + _("\" with id ") + std::to_string(id) +
                       _(" loaded from file. Spawned at x: ") + std::to_string(getCenterGridPosition().x) +
                       ", y: " + std::to_string(getCenterGridPosition().y));
    }
    else
    {
        createMovementFunctionality(100.f, MovementAllow::AllowAll);
        createAttributeFunctionality(20, 20);

        logger.logInfo(_("Player \"") + name + _("\" with id ") + std::to_string(id) + _(" spawned at x: ") +
                       std::to_string(getCenterGridPosition().x) + ", y: " + std::to_string(getCenterGridPosition().y));
    }
}

Player::~Player() = default;

void Player::update(const float &dt, const bool &update_movement, const std::string &tile_name_under)
{
    movementFunctionality->update();

    if (update_movement)
    {
        if (collisionRect.has_value())
        {
            uint8_t movement_direction = movementFunctionality->getDirection();

            if (movement_direction == Up && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                setHitBoxPosition(
                    sf::Vector2f(getFirstHitBoxPosition().x, collisionRect->position.y + collisionRect->size.y + 1.f));

            else if (movement_direction == Down && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                setHitBoxPosition(
                    sf::Vector2f(getFirstHitBoxPosition().x, collisionRect->position.y - getFirstHitBoxSize().y - 1.f));

            else if (movement_direction == Left && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                setHitBoxPosition(
                    sf::Vector2f(collisionRect->position.x + collisionRect->size.x + 1.f, getFirstHitBoxPosition().y));

            else if (movement_direction == Right && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                setHitBoxPosition(
                    sf::Vector2f(collisionRect->position.x - getFirstHitBoxSize().x - 1.f, getFirstHitBoxPosition().y));

            else
                collisionRect.reset();
        }

        if (!collisionRect.has_value())
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                move(dt, Up);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                move(dt, Down);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                move(dt, Left);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                move(dt, Right);
        }
    }

    collisionFunctionality->update();

    const uint8_t mov_state = movementFunctionality->getState();
    const std::string mov_direction = movementFunctionality->getDirectionAsString();

    if (mov_state == MovementState::Idle)
        animationFunctionality->play("Idle" + mov_direction, true);

    else if (mov_state == MovementState::Walking)
    {
        animationFunctionality->play("Walk" + mov_direction, true);

        if (walkSoundClock.getElapsedTime().asMilliseconds() >= walkTimeMax)
        {
            walkSoundClock.restart();

            int i = (std::rand() % 5) + 1;

            if (soundFunctionality->exists(tile_name_under + "Walk" + std::to_string(i)))
                soundFunctionality->playSound(tile_name_under + "Walk" + std::to_string(i));
            else
                soundFunctionality->playSound("StoneWalk" + std::to_string(i));
        }
    }
}

void Player::update(const float &dt, const sf::Vector2f &mouse_pos)
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

    const uint8_t mov_state = movementFunctionality->getState();
    const std::string mov_direction = movementFunctionality->getDirectionAsString();

    if (mov_state == MovementState::Idle)
        animationFunctionality->play("Idle" + mov_direction, true);

    else if (mov_state == MovementState::Walking)
    {
        animationFunctionality->play("Walk" + mov_direction, true);

        if (soundFunctionality->getSoundStatus("GrassWalk1") != sf::Sound::Status::Playing)
            soundFunctionality->playSound("GrassWalk1");
    }
}

void Player::render(sf::RenderTarget &target)
{
    for (auto &[_, sprite] : layers)
    {
        if (sprite)
            target.draw(*sprite);
    }
}

void Player::render(sf::RenderTarget &target, const bool &show_hitboxes)
{
    for (auto &[_, sprite] : layers)
    {
        if (sprite)
            target.draw(*sprite);
    }

    if (show_hitboxes)
    {
        for (auto &[_, hitbox] : getHitBoxes())
        {
            target.draw(hitbox.rect);
            target.draw(hitbox.predictNextPos(movementFunctionality->getVelocity()).rect);
        }
    }
}

void Player::save(const std::string &folder_name, const std::string &uuid)
{
    const std::filesystem::path root = MAPS_FOLDER + folder_name;

    if (!std::filesystem::exists(root))
        logger.logError(_("Failed to save ") + name + _(" to world ") + folder_name + _(": Folder does not exist."));

    if (!std::filesystem::exists(root / "players/"))
        std::filesystem::create_directory(root / "players/");

    preparePlayerData(uuid);

    std::ofstream player_data_file(root / "players" / (uuid + ".dat"));

    if (!player_data_file.is_open())
        throw std::runtime_error(_("Failed to save player data: ") + name);

    player_data_file.write(reinterpret_cast<char *>(&playerData.currentGridPosition.x), sizeof(float));
    player_data_file.write(reinterpret_cast<char *>(&playerData.currentGridPosition.y), sizeof(float));

    player_data_file.write(reinterpret_cast<char *>(&playerData.spawnGridPosition.x), sizeof(float));
    player_data_file.write(reinterpret_cast<char *>(&playerData.spawnGridPosition.y), sizeof(float));

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
        logger.logError(_("Something went wrong while saving player data: ") + name + _(" (bad bit was set)."));
    }

    player_data_file.close();

    logger.logInfo(_("Player data saved to player: ") + name);
}
