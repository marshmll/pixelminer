#include "States/Singleplayer/GameState.hxx"
#include "stdafx.hxx"

void GameState::initLoadingScreen()
{
    loadingBg.setSize(sf::Vector2f(data.vm->size));
    loadingBg.setPosition({0, 0});
    loadingBg.setTexture(&data.activeResourcePack->getTexture("Background"));

    std::string msg = _("Loading world");

    loadingText = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"),
                                             sf::String::fromUtf8(msg.begin(), msg.end()), gui::charSize(*data.vm, 85));

    loadingText->setPosition(
        sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - loadingText->getGlobalBounds().size.x / 2.f),
                     static_cast<int>(gui::percent(data.vm->size.y, 45.f))));

    loadingMsg =
        std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"), "", gui::charSize(*data.vm, 85));
    loadingMsg->setPosition(
        sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - loadingMsg->getGlobalBounds().size.x / 2.f),
                     static_cast<int>(gui::percent(data.vm->size.y, 50.f))));

    loaderSprite = std::make_unique<sf::Sprite>(data.activeResourcePack->getTexture("Loader"));
    loaderSprite->setScale(sf::Vector2f(data.scale, data.scale));
    loaderSprite->setPosition(sf::Vector2f(gui::percent(data.vm->size.x, 50.f) - (16.f * data.scale) / 2.f,
                                           gui::percent(data.vm->size.y, 60.f)));

    loaderAnimation = std::make_unique<Animation>(*loaderSprite, data.activeResourcePack->getTexture("Loader"), 100,
                                                  sf::Vector2u(19, 7), sf::Vector2u(0, 0), sf::Vector2u(5, 0), true);
}

void GameState::initMap()
{
    ctx.map = std::make_unique<Map>(data.activeResourcePack->tileDb, data.activeResourcePack->getTexture("TileSheet"),
                                    data.gridSize, data.scale);
}

void GameState::initMap(const std::string &map_folder_name)
{
    ctx.map = std::make_unique<Map>(data.activeResourcePack->tileDb, data.activeResourcePack->getTexture("TileSheet"),
                                    data.gridSize, data.scale);
    if (!map_folder_name.empty())
        ctx.map->load(map_folder_name);
}

void GameState::initEntitySpatialGridPartition()
{
    entitySpacialGridPartition = std::make_unique<EntitySpatialGridPartition>(data.scale);
}

void GameState::initThisPlayer()
{
    ctx.globalEntities.emplace_back(
        std::make_shared<Player>("marshmll", ctx.map->getFolderName(), data.uuid, ctx.map->getSpawnPoint(),
                                 data.activeResourcePack->getTexture("Player1"), data.scale));
    ctx.players[data.uuid] = std::static_pointer_cast<Player>(ctx.globalEntities.back());
    thisPlayer = ctx.players[data.uuid];
    entitySpacialGridPartition->put(ctx.globalEntities.back());
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

void GameState::initChat()
{
    chat = std::make_unique<gui::Chat>(
        thisPlayer->getName(), sf::Vector2f(gui::percent(data.vm->size.x, 35.f), gui::percent(data.vm->size.y, 25.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 2.f), gui::percent(data.vm->size.y, 2.f)),
        data.activeResourcePack->getFont("Regular"), *data.vm);
}

void GameState::initPauseMenu()
{
    pauseMenu = std::make_unique<gui::PauseMenu>(data, server, *chat);
}

void GameState::initCommandInterpreter()
{
    cmd = std::make_unique<CommandInterpreter>(ctx);
}

void GameState::initDebugging()
{
    debugChunks = debugInfo = debugHitBoxes = false;
    debugText =
        std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"), "", gui::charSize(*data.vm, 130));
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

GameState::GameState(EngineData &data) : State(data), server(data.uuid)
{
    ctx.currentState = this;
    initLoadingScreen();
    initMap();
    initEntitySpatialGridPartition();
    initThisPlayer();
    initPlayerGUI();
    initPlayerCamera();
    initChat();
    initPauseMenu();
    initCommandInterpreter();
    initDebugging();
}

GameState::GameState(EngineData &data, const std::string &map_folder_name) : State(data), server(data.uuid)
{
    ctx.currentState = this;
    initLoadingScreen();
    initMap(map_folder_name);
    initEntitySpatialGridPartition();
    initThisPlayer();
    initPlayerGUI();
    initPlayerCamera();
    initChat();
    initPauseMenu();
    initCommandInterpreter();
    initDebugging();

    ctx.globalEntities.emplace_back(std::make_shared<PineTree>(
        ctx.map->getSpawnPoint(), data.activeResourcePack->getTexture("PineTree"), data.scale));
    entitySpacialGridPartition->put(ctx.globalEntities.back());
}

GameState::~GameState() = default;

void GameState::update(const float &dt)
{
    if (!ctx.map->isReady())
    {
        std::string msg = ctx.map->getMessage();

        loadingMsg->setString(sf::String::fromUtf8(msg.begin(), msg.end()));
        loadingMsg->setPosition(
            sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - loadingMsg->getGlobalBounds().size.x / 2.f),
                         static_cast<int>(gui::percent(data.vm->size.y, 50.f))));
        loaderAnimation->play();
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

    updateMousePositions(playerCamera);
    if (!chat->isActive())
        playerGUI->update(dt, mousePosGrid);

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
    ctx.map->update(dt, sf::Vector2i(thisPlayer->getCenterGridPosition()));
}

void GameState::updateGlobalEntities(const float &dt)
{
    for (auto &entity : ctx.globalEntities)
    {
        if (entity && entity->getType() != EntityType::PlayerEntity)
            entity->update(dt, mousePosView);
    }
}

void GameState::updatePlayers(const float &dt)
{
    for (auto &[_, player] : ctx.players)
        player->update(dt, player.get() == thisPlayer.get() && !chat->isActive());
}

void GameState::updateEntityRenderPriorityQueue()
{
    for (auto &entity : ctx.globalEntities)
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
    sf::Vector2f mapDimensions = ctx.map->getRealDimensions();

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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && keyPressedWithin(250, sf::Keyboard::Key::C))
        chat->setActive(!chat->isActive());
    else if (keyPressedWithin(250, sf::Keyboard::Key::Enter) && chat->isActive())
    {
        std::string value = chat->getInputValue();

        if (value.size() > 0)
        {
            if (chat->getInputValue()[0] == COMMAND_INVOKER_CHAR)
            {
                const std::string msg = cmd->interpret("marshmll", data.uuid, value);
                chat->displayGameLog(msg);
                chat->setActive(false);
                chat->clearInput();
            }
            else
                chat->sendMessageFromInput();
        }
    }

    chat->update(dt, mousePosView, *data.event);
}

void GameState::updateDebugText(const float &dt)
{
    std::stringstream ss;
    ss << static_cast<int>(1.f / dt) << " fps\n"
       << std::fixed << std::setprecision(5) << dt << " ms\n"
       << _("grid x, y: ") << thisPlayer->getCenterGridPosition().x << " | " << thisPlayer->getCenterGridPosition().y
       << "\n"
       << _("velocity x, y: ") << std::round(thisPlayer->getVelocity().x / data.scale / dt) << " | "
       << std::round(thisPlayer->getVelocity().y / data.scale / dt) << "\n"
       << _("chunk:") << " ["
       << static_cast<unsigned int>(thisPlayer->getCenter().x / (CHUNK_SIZE_IN_TILES.x * data.gridSize * data.scale))
       << ", "
       << static_cast<unsigned int>(thisPlayer->getCenter().y / (CHUNK_SIZE_IN_TILES.y * data.gridSize * data.scale))
       << "]\n"
       << _("region:") << " ["
       << static_cast<unsigned int>(thisPlayer->getCenter().x /
                                    (REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x * data.gridSize * data.scale))
       << ", "
       << static_cast<unsigned int>(thisPlayer->getCenter().y /
                                    (REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y * data.gridSize * data.scale))
       << "]\n"
       << _("biome:") << " " << ctx.map->getBiomeAt(sf::Vector2i(thisPlayer->getCenterGridPosition())).name << "\n"
       << _("height: ") << ctx.map->getHeightAt(sf::Vector2i(thisPlayer->getCenterGridPosition())) << _(", moisture: ")
       << ctx.map->getMoistureAt(sf::Vector2i(thisPlayer->getCenterGridPosition())) << _(", heat: ")
       << ctx.map->getHeatAt(sf::Vector2i(thisPlayer->getCenterGridPosition())) << "\n";

    std::string str = ss.str();
    debugText->setString(sf::String::fromUtf8(str.begin(), str.end()));
}

void GameState::render(sf::RenderTarget &target)
{
    // DO NOT RENDER DIRECTLY TO TARGET!

    renderTexture.clear();

    if (!ctx.map->isReady())
    {
        target.draw(loadingBg);
        target.draw(*loadingText);
        target.draw(*loadingMsg);
        target.draw(*loaderSprite);
        return;
    }

    renderTexture.setView(playerCamera);
    ctx.map->render(renderTexture, sf::Vector2i(thisPlayer->getCenterGridPosition()), debugChunks);

     if (!chat->isActive())
        playerGUI->renderTileHoverIndicator(renderTexture);

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
    ctx.map->save();
    for (auto &[uuid, player] : ctx.players)
        player->save(ctx.map->getFolderName(), uuid);
}
