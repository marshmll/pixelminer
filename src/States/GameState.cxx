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
    loadingText->setPosition(sf::Vector2f(data.vm->size.x / 2.f - loadingText->getGlobalBounds().size.x / 2.f,
                                          gui::percent(data.vm->size.y, 45.f)));

    loadingMsg =
        std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Regular"), "", gui::charSize(*data.vm, 80));
    loadingMsg->setPosition(sf::Vector2f(data.vm->size.x / 2.f - loadingMsg->getGlobalBounds().size.x / 2.f,
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
    if (!map_folder_name.empty())
        map->load(map_folder_name);
}

void GameState::initEntitySpatialGridPartition()
{
    entitySpacialGridPartition = std::make_unique<EntitySpatialGridPartition>(data.scale);
}

void GameState::initThisPlayer()
{
    globalEntities.emplace_back(std::make_shared<Player>("marshmll", map->getFolderName(), data.uuid,
                                                         map->getSpawnPoint(),
                                                         data.activeResourcePack->getTexture("Player1"), data.scale));
    players[data.uuid] = std::static_pointer_cast<Player>(globalEntities.back());
    thisPlayer = players[data.uuid];
    entitySpacialGridPartition->put(globalEntities.back());
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
    debugText->setPosition(sf::Vector2f(gui::percent(data.vm->size.x, 1.f), gui::percent(data.vm->size.y, 1.f)));
}

void GameState::resolveCollision(std::shared_ptr<Entity> &first_entity, std::shared_ptr<Entity> &second_entity,
                                 const sf::FloatRect &intersection)
{
    if (!first_entity->canMove())
        return;

    float overlapX = intersection.size.x;
    float overlapY = intersection.size.y;
    sf::Vector2f first_pos = first_entity->getPosition();
    sf::Vector2f second_pos = second_entity->getPosition();

    if (overlapX < overlapY)
        first_entity->move(sf::Vector2f((first_pos.x < second_pos.x ? -overlapX : overlapX), 0.f));
    else
        first_entity->move(sf::Vector2f(0.f, (first_pos.y < second_pos.y ? -overlapY : overlapY)));
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

    globalEntities.emplace_back(std::make_shared<PineTree>(
        map->getSpawnPoint() + sf::Vector2f(2.f, 2.f), data.activeResourcePack->getTexture("PineTree"), data.scale));
    entitySpacialGridPartition->put(globalEntities.back());

    globalEntities.emplace_back(std::make_shared<PineTree>(
        map->getSpawnPoint() + sf::Vector2f(4.f, 4.f), data.activeResourcePack->getTexture("PineTree"), data.scale));
    entitySpacialGridPartition->put(globalEntities.back());

    globalEntities.emplace_back(std::make_shared<PineTree>(
        map->getSpawnPoint() + sf::Vector2f(6.f, 6.f), data.activeResourcePack->getTexture("PineTree"), data.scale));
    entitySpacialGridPartition->put(globalEntities.back());

    globalEntities.emplace_back(std::make_shared<PineTree>(
        map->getSpawnPoint() + sf::Vector2f(8.f, 8.f), data.activeResourcePack->getTexture("PineTree"), data.scale));
    entitySpacialGridPartition->put(globalEntities.back());
}

GameState::~GameState() = default;

void GameState::update(const float &dt)
{
    if (!map->isReady())
    {
        loadingMsg->setString(map->getMessage());
        loadingMsg->setPosition(sf::Vector2f(data.vm->size.x / 2.f - loadingMsg->getGlobalBounds().size.x / 2.f,
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
    updateGlobalEntities(dt);
    updatePlayers(dt);
    updateCollisions(dt);
    updatePlayerCamera();
    updateChat(dt);
    updateEntityRenderPriorityQueue();

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
        debugInfo = !debugInfo;
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
    {
        if (entity && entity->getType() != EntityType::PlayerEntity)
            entity->update(dt, mousePosView);
    }
}

void GameState::updatePlayers(const float &dt)
{
    for (auto &[_, player] : players)
        player->update(dt, player.get() == thisPlayer.get() && !chat->isActive());
}

void GameState::updateEntityRenderPriorityQueue()
{
    for (auto &entity : globalEntities)
        if (entity)
            entityRenderPriorityQueue.push(entity);
}

void GameState::updateCollisions(const float &dt)
{
    // Get current and previous cell coordinates of the player
    const sf::Vector2i prev_cell_coords = entitySpacialGridPartition->getEntityCellGridCoords(thisPlayer);
    const sf::Vector2i curr_cell_coords = entitySpacialGridPartition->calcEntityCellGridCoords(thisPlayer);
    const auto &[curr_x, curr_y] = curr_cell_coords;

    // Move the player to the new cell if the coordinates have changed
    if (curr_cell_coords != prev_cell_coords)
        entitySpacialGridPartition->move(thisPlayer, curr_cell_coords);

    Cell &current_cell = entitySpacialGridPartition->getCell(curr_x, curr_y);

    // Track pairs of entities that have already been checked to avoid redundant checks
    std::unordered_set<uint64_t> processedPairs;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            const int neighbour_x = curr_x + dx;
            const int neighbour_y = curr_y + dy;

            if (neighbour_x < 0 || neighbour_x >= SPATIAL_GRID_PARTITION_DIMENSIONS.x || neighbour_y < 0 ||
                neighbour_y >= SPATIAL_GRID_PARTITION_DIMENSIONS.y)
                continue;

            Cell &neighbour_cell = entitySpacialGridPartition->getCell(neighbour_x, neighbour_y);

            // Compare all entities
            for (auto &first_entity : neighbour_cell)
            {
                if (!first_entity)
                    continue;

                for (auto &second_entity : current_cell)
                {
                    if (!second_entity || first_entity == second_entity)
                        continue;

                    // Generate two unique keys for the pair to avoid redundant checks between two entities, regardless
                    // of order.
                    const uint64_t first_pair_key =
                        (static_cast<uint64_t>(first_entity->getId()) << 32) | second_entity->getId();
                    const uint64_t second_pair_key =
                        (static_cast<uint64_t>(second_entity->getId()) << 32) | first_entity->getId();

                    if (processedPairs.count(first_pair_key) || processedPairs.count(second_pair_key))
                        continue;

                    processedPairs.insert(first_pair_key);
                    processedPairs.insert(second_pair_key);

                    // Check collisions between hitboxes of the two entities
                    for (auto &[_, first_hitbox] : first_entity->getHitBoxes())
                    {
                        for (auto &[_, second_hitbox] : second_entity->getHitBoxes())
                        {
                            HitBox next_first_hitbox = first_hitbox.predictNextPos(first_entity->getVelocity());
                            if (auto intersection = next_first_hitbox.findIntersection(second_hitbox))
                            {
                                first_entity->setCollisionRect(intersection);
                                second_entity->setCollisionRect(intersection);
                                resolveCollision(first_entity, second_entity, intersection.value());
                            }
                        }
                    }
                }
            }
        }
    }
}

void GameState::updatePlayerCamera()
{
    playerCamera.setCenter(thisPlayer->getCenter());
    sf::Vector2f cameraCenter = playerCamera.getCenter();
    sf::Vector2f cameraSize = playerCamera.getSize();
    sf::Vector2f mapDimensions = map->getRealDimensions();

    if (cameraCenter.x - cameraSize.x / 2.f <= 0.f)
        cameraCenter.x = cameraSize.x / 2.f;
    else if (cameraCenter.x + cameraSize.x / 2.f >= mapDimensions.x)
        cameraCenter.x = mapDimensions.x - cameraSize.x / 2.f;

    if (cameraCenter.y - cameraSize.y / 2.f <= 0.f)
        cameraCenter.y = cameraSize.y / 2.f;
    else if (cameraCenter.y + cameraSize.y / 2.f >= mapDimensions.y)
        cameraCenter.y = mapDimensions.y - cameraSize.y / 2.f;

    playerCamera.setCenter(cameraCenter);
}

void GameState::updateChat(const float &dt)
{
    if (!chat->isActive())
        playerGUI->update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && keyPressedWithin(250, sf::Keyboard::Key::C))
        chat->setActive(!chat->isActive());
    else if (keyPressedWithin(250, sf::Keyboard::Key::Enter) && chat->isActive())
        chat->sendMessageFromInput();

    chat->update(dt, mousePosView, *data.event, data.mouseData);
}

void GameState::updateDebugText(const float &dt)
{
    std::stringstream sstream;
    sstream << static_cast<int>(1.f / dt) << " fps\n"
            << std::fixed << std::setprecision(5) << dt << " ms\n"
            << "grid x, y: " << thisPlayer->getCenterGridPosition().x << " | " << thisPlayer->getCenterGridPosition().y
            << "\n"
            << "velocity x, y: " << std::round(thisPlayer->getVelocity().x / data.scale / dt) << " | "
            << std::round(thisPlayer->getVelocity().y / data.scale / dt) << "\n"
            << "chunk: ["
            << static_cast<unsigned int>(thisPlayer->getCenter().x /
                                         (CHUNK_SIZE_IN_TILES.x * data.gridSize * data.scale))
            << ", "
            << static_cast<unsigned int>(thisPlayer->getCenter().y /
                                         (CHUNK_SIZE_IN_TILES.y * data.gridSize * data.scale))
            << "]\n"
            << "region: ["
            << static_cast<unsigned int>(thisPlayer->getCenter().x /
                                         (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x * data.gridSize * data.scale))
            << ", "
            << static_cast<unsigned int>(thisPlayer->getCenter().y /
                                         (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y * data.gridSize * data.scale))
            << "]\n"
            << "biome: " << map->getBiomeAt(sf::Vector2i(thisPlayer->getCenterGridPosition())).name << "\n"
            << "height: " << map->getHeightAt(sf::Vector2i(thisPlayer->getCenterGridPosition()))
            << ", moisture: " << map->getHeightAt(sf::Vector2i(thisPlayer->getCenterGridPosition()))
            << ", heat: " << map->getHeatAt(sf::Vector2i(thisPlayer->getCenterGridPosition())) << "\n";

    debugText->setString(sstream.str());
}

void GameState::render(sf::RenderTarget &target)
{
    renderTexture.clear();

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

    renderTexture.setView(renderTexture.getDefaultView());
    if (!chat->isActive())
        playerGUI->render(renderTexture);

    if (!pauseMenu->isActive())
    {
        chat->render(renderTexture);
        if (debugInfo)
            renderTexture.draw(*debugText);
    }

    pauseMenu->render(renderTexture);
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());
    target.draw(renderSprite);
}

void GameState::renderGlobalEntities(sf::RenderTarget &target)
{
    while (!entityRenderPriorityQueue.empty())
    {
        entityRenderPriorityQueue.top()->render(target, debugHitBoxes);
        entityRenderPriorityQueue.pop();
    }
}

void GameState::saveWorld()
{
    map->save();
    for (auto &[uuid, player] : players)
        player->save(map->getFolderName(), uuid);
}