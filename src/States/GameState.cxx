#include "States/GameState.hxx"
#include "stdafx.hxx"

void GameState::initLoadingScreen()
{
    loadingBg.setSize(sf::Vector2f(data.vm->size));
    loadingBg.setPosition({0, 0});
    loadingBg.setTexture(&data.activeResourcePack->getTexture("Background"));
    loadingBg.setFillColor(sf::Color(255, 255, 255, 80));

    loadingText = std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Regular"), "Loading world",
                                             gui::charSize(*data.vm, 80));

    loadingText->setPosition(
        sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - loadingText->getGlobalBounds().size.x / 2.f),
                     gui::percent(data.vm->size.y, 45.f)));

    loadingMsg =
        std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Regular"), "", gui::charSize(*data.vm, 80));

    loadingMsg->setPosition(
        sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - loadingMsg->getGlobalBounds().size.x / 2.f),
                     gui::percent(data.vm->size.y, 50.f)));
}

void GameState::initMap()
{
    map = std::make_unique<Map>(data.activeResourcePack->tileDB, data.activeResourcePack->getTexture("TileSheet"),
                                data.gridSize, data.scale);
}

void GameState::initMap(const std::string &map_folder_name)
{
    map = std::make_unique<Map>(data.activeResourcePack->tileDB, data.activeResourcePack->getTexture("TileSheet"),
                                data.gridSize, data.scale);

    map->load(map_folder_name);
}

void GameState::initEntitySpatialGridPartition()
{
    entitySpacialGridPartition = std::make_unique<EntitySpacialGridPartition>(data.scale);
}

void GameState::initThisPlayer()
{
    players[data.uuid] = std::make_shared<Player>("marshmll", map->getFolderName(), data.uuid, map->getSpawnPoint(),
                                                  data.activeResourcePack->getTexture("Player1"), data.scale);

    thisPlayer = players.at(data.uuid);

    entitySpacialGridPartition->put(std::reinterpret_pointer_cast<Entity>(thisPlayer));
}

void GameState::initPlayerGUI()
{
    playerGUI = std::make_unique<PlayerGUI>(*thisPlayer, data.activeResourcePack, *data.vm, data.scale);
}

void GameState::initPlayerCamera()
{
    playerCamera.setSize(sf::Vector2f(data.vm->size));
    playerCamera.setCenter(thisPlayer->getCenter());
}

void GameState::initPauseMenu()
{
    pauseMenu = std::make_unique<gui::PauseMenu>(data);
}

void GameState::initChat()
{
    chat = std::make_unique<gui::Chat>(
        thisPlayer->getName(), sf::Vector2f(gui::percent(data.vm->size.x, 30.f), gui::percent(data.vm->size.y, 25.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 2.f), gui::percent(data.vm->size.y, 2.f)),
        data.activeResourcePack->fonts.at("Regular"), *data.vm);
}

void GameState::initDebugging()
{
    debugChunks = debugInfo = debugHitBoxes = false;

    debugText =
        std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Italic"), "", gui::charSize(*data.vm, 130));
    debugText->setPosition(
        sf::Vector2f((int)gui::percent(data.vm->size.x, 1.f), (int)gui::percent(data.vm->size.y, 1.f)));
}

// Helper function to resolve collisions
void GameState::resolveCollision(std::shared_ptr<Entity> &first_entity, std::shared_ptr<Entity> &second_entity,
                                 const sf::FloatRect &intersection)
{
    if (!first_entity->isMovable())
        return;

    // Calculate the overlap in the X and Y directions
    float overlapX = intersection.size.x;
    float overlapY = intersection.size.y;

    // Determine the direction of the collision
    sf::Vector2f first_pos = first_entity->getPosition();
    sf::Vector2f second_pos = second_entity->getPosition();

    if (overlapX < overlapY)
    {
        // Resolve collision on the X axis
        if (first_pos.x < second_pos.x)
            first_entity->move(sf::Vector2f(-overlapX, 0.f));
        else
            first_entity->move(sf::Vector2f(overlapX, 0.f));
    }
    else
    {
        // Resolve collision on the Y axis
        if (first_pos.y < second_pos.y)
            first_entity->move(sf::Vector2f(0.f, -overlapY));
        else
            first_entity->move(sf::Vector2f(0.f, overlapY));
    }
}

GameState::GameState(EngineData &data) : State(data), client(data.uuid), server(data.uuid)
{
    initLoadingScreen();
    initMap();
    initEntitySpatialGridPartition();
    initThisPlayer();
    initPlayerGUI();
    initPlayerCamera();
    initPauseMenu();
    initDebugging();
}

GameState::GameState(EngineData &data, const std::string &map_folder_name)
    : State(data), client(data.uuid), server(data.uuid)
{
    initLoadingScreen();
    initMap(map_folder_name);
    initEntitySpatialGridPartition();
    initThisPlayer();
    initPlayerGUI();
    initPlayerCamera();
    initPauseMenu();
    initChat();
    initDebugging();

    globalEntities.emplace_back(
        std::make_shared<PineTree>(map->getSpawnPoint(), data.activeResourcePack->getTexture("PineTree"), data.scale));

    entitySpacialGridPartition->put(globalEntities.back());
}

GameState::~GameState()
{
}

void GameState::update(const float &dt)
{
    if (!map->isReady())
    {
        loadingMsg->setString(map->getMessage());
        loadingMsg->setPosition(
            sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - loadingMsg->getGlobalBounds().size.x / 2.f),
                         gui::percent(data.vm->size.y, 50.f)));
        return;
    }

    updateMousePositions();
    updatePauseMenu(dt);

    if (pauseMenu->isActive())
    {
        if (pauseMenu->getQuit())
        {
            saveWorld();
            killSelf();
        }

        return;
    }

    updateMap(dt);
    updateCollisions(dt);
    updateGlobalEntities(dt);
    updatePlayers(dt);
    updatePlayerCamera();
    updateChat(dt);

    if (debugInfo)
        updateDebugText(dt);

    // Debug
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        if (keyPressedWithin(250, sf::Keyboard::Key::F1))
            debugChunks = !debugChunks;
        else if (keyPressedWithin(250, sf::Keyboard::Key::F2))
            debugHitBoxes = !debugHitBoxes;
    }
    else if (keyPressedWithin(250, sf::Keyboard::Key::F3))
    {
        debugInfo = !debugInfo;
    }
}

void GameState::updatePauseMenu(const float &dt)
{
    if (keyPressedWithin(250, sf::Keyboard::Key::Escape))
        pauseMenu->toggleActive();

    pauseMenu->update(dt, mousePosView);
}

void GameState::updateMap(const float &dt)
{
    map->update(dt, sf::Vector2i(thisPlayer->getCenterGridPosition()));
}

void GameState::updateGlobalEntities(const float &dt)
{
    for (auto &entity : globalEntities)
        if (entity)
            entity->update(dt, mousePosView);
}

void GameState::updatePlayers(const float &dt)
{
    for (auto &[uuid, player] : players)
        player->update(dt, player.get() == thisPlayer.get() && !chat->isActive());
}

void GameState::updateCollisions(const float &dt)
{
    // Update the entity's position in the spatial grid
    auto [prev_x, prev_y] = entitySpacialGridPartition->getEntitySpatialGridCoords(thisPlayer);
    auto [curr_x, curr_y] = entitySpacialGridPartition->calculateSpatialGridCoords(thisPlayer);

    if (prev_x != curr_x || prev_y != curr_y)
        entitySpacialGridPartition->move(thisPlayer, curr_x, curr_y);

    // Get the current cell of the player
    auto &currentCell = entitySpacialGridPartition->getCell(curr_x, curr_y);

    // Iterate over all entities in the current cell
    for (size_t i = 0; i < currentCell.size(); ++i)
    {
        auto &first_entity = currentCell[i];
        if (!first_entity)
            continue;

        // Check collisions with other entities in the same cell
        for (size_t j = i + 1; j < currentCell.size(); ++j)
        {
            auto &second_entity = currentCell[j];
            if (!second_entity || first_entity == second_entity)
                continue;

            // Check collisions between all hitboxes of the two entities
            for (auto &[_, first_hitbox] : first_entity->getHitBoxes())
            {
                for (auto &[_, second_hitbox] : second_entity->getHitBoxes())
                {
                    // Predict the next position of the first entity's hitbox
                    HitBox next_first_hitbox = first_hitbox.predictNextPos(first_entity->getVelocity());

                    // Check for intersection
                    std::optional<sf::FloatRect> intersection = next_first_hitbox.findIntersection(second_hitbox);

                    if (intersection)
                    {
                        // Set the collision rectangle for both entities
                        first_entity->setCollisionRect(intersection);
                        second_entity->setCollisionRect(intersection);

                        // Resolve the collision by adjusting the first entity's position
                        resolveCollision(first_entity, second_entity, intersection.value());
                    }
                }
            }
        }
    }
}

void GameState::updatePlayerCamera()
{
    playerCamera.setCenter(thisPlayer->getCenter());

    if ((playerCamera.getCenter().x - playerCamera.getSize().x / 2.f) <= 0.f)
    {
        playerCamera.setCenter(sf::Vector2f(playerCamera.getSize().x / 2.f, playerCamera.getCenter().y));
    }
    else if ((playerCamera.getCenter().x + playerCamera.getSize().x / 2.f) >= map->getRealDimensions().x)
    {
        playerCamera.setCenter(
            sf::Vector2f(map->getRealDimensions().x - playerCamera.getSize().x / 2.f, playerCamera.getCenter().y));
    }
    if ((playerCamera.getCenter().y - playerCamera.getSize().y / 2.f) <= 0.f)
    {
        playerCamera.setCenter(sf::Vector2f(playerCamera.getCenter().x, playerCamera.getSize().y / 2.f));
    }
    else if ((playerCamera.getCenter().y + playerCamera.getSize().y / 2.f) >= map->getRealDimensions().y)
    {
        playerCamera.setCenter(
            sf::Vector2f(playerCamera.getCenter().x, map->getRealDimensions().y - playerCamera.getSize().y / 2.f));
    }
}

void GameState::updateChat(const float &dt)
{
    if (!chat->isActive())
        playerGUI->update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
    {
        if (keyPressedWithin(250, sf::Keyboard::Key::C))
            chat->setActive(!chat->isActive());
    }

    else if (keyPressedWithin(250, sf::Keyboard::Key::Enter) && chat->isActive())
        chat->sendMessageFromInput();

    chat->update(dt, mousePosView, *data.event, data.mouseData);
}

void GameState::updateDebugText(const float &dt)
{
    std::stringstream sstream;

    sstream << static_cast<int>(1.f / dt) << " fps" << "\n"
            << std::fixed << std::setprecision(5) << dt << " ms" << "\n"
            << "grid x, y: " << std::fixed << std::setprecision(5) << thisPlayer->getCenterGridPosition().x << " | "
            << std::fixed << std::setprecision(5) << thisPlayer->getCenterGridPosition().y << "\n"
            << "velocity x, y: " << std::fixed << std::setprecision(5)
            << std::round(thisPlayer->getVelocity().x / data.scale / dt) << " | " << std::fixed << std::setprecision(5)
            << std::round(thisPlayer->getVelocity().y / data.scale / dt) << "\n"
            << "chunk: ["
            << static_cast<unsigned int>(thisPlayer->getCenter().x /
                                         (CHUNK_SIZE_IN_TILES.x * data.gridSize * data.scale))
            << ", "
            << static_cast<unsigned int>(thisPlayer->getCenter().y /
                                         (CHUNK_SIZE_IN_TILES.y * data.gridSize * data.scale))
            << "]"
            << "\n"
            << "region: ["
            << static_cast<unsigned int>(thisPlayer->getCenter().x /
                                         (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x * data.gridSize * data.scale))
            << ", "
            << static_cast<unsigned int>(thisPlayer->getCenter().y /
                                         (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y * data.gridSize * data.scale))
            << "]" << "\n"
            << "biome: " << map->getBiomeAt(sf::Vector2i(thisPlayer->getCenterGridPosition())).name << "\n"
            << "height: " << map->getHeightAt(sf::Vector2i(thisPlayer->getCenterGridPosition()))
            << ", moisture: " << map->getHeightAt(sf::Vector2i(thisPlayer->getCenterGridPosition()))
            << ", heat: " << map->getHeatAt(sf::Vector2i(thisPlayer->getCenterGridPosition())) << "\n";

    debugText->setString(sstream.str());
}

void GameState::render(sf::RenderTarget &target)
{
    renderTexture.clear(); // DO NOT FORGET TO RENDER TO THE RENDER TEXTURE!

    if (!map->isReady())
    {
        target.draw(loadingBg);
        target.draw(*loadingText);
        target.draw(*loadingMsg);
        return;
    }

    renderTexture.setView(playerCamera);

    map->render(renderTexture, sf::Vector2i(thisPlayer->getCenterGridPosition()), debugChunks);

    renderGlobalEntities(renderTexture);

    for (auto &[name, player] : players)
        player->render(renderTexture, debugHitBoxes);

    renderTexture.setView(renderTexture.getDefaultView());

    if (!chat->isActive())
        playerGUI->render(renderTexture);

    if (!pauseMenu->isActive())
        chat->render(renderTexture);

    if (!pauseMenu->isActive() && debugInfo)
        renderTexture.draw(*debugText);

    pauseMenu->render(renderTexture);

    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());

    target.draw(renderSprite);
}

void GameState::renderGlobalEntities(sf::RenderTarget &target)
{
    for (auto &entity : globalEntities)
    {
        if (entity)
            entity->render(target, debugHitBoxes);
    }
}

void GameState::saveWorld()
{
    map->save();

    for (auto &[uuid, player] : players)
        player->save(map->getFolderName(), uuid);
}