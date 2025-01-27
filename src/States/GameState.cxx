#include "States/GameState.hxx"
#include "stdafx.hxx"

void GameState::initLoadingScreen()
{
    loadingBg.setSize(sf::Vector2f(data.vm->size));
    loadingBg.setPosition({0, 0});
    loadingBg.setTexture(&data.activeResourcePack->textures.at("Background"));
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
    map = std::make_unique<Map>("My New World", 56465456464654, data.activeResourcePack->tileDB,
                                data.activeResourcePack->textures.at("TileSheet"), data.gridSize, data.scale);
}

void GameState::initThisPlayer()
{
    players[data.uuid] =
        std::make_shared<Player>(map->getSpawnPoint(), data.activeResourcePack->textures.at("Player1"), data.scale);
    thisPlayer = players.at(data.uuid);
}

void GameState::initPlayerCamera()
{
    playerCamera.setSize(sf::Vector2f(data.vm->size));
    playerCamera.setCenter(thisPlayer->getCenter());
}

void GameState::initPauseMenu()
{
    pauseMenu = std::make_unique<gui::PauseMenu>(data, *map);
}

void GameState::initDebugging()
{
    debugChunks = debugInfo = false;

    debugText =
        std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Italic"), "0", gui::charSize(*data.vm, 130));
    debugText->setPosition(
        sf::Vector2f((int)gui::percent(data.vm->size.x, 1.f), (int)gui::percent(data.vm->size.y, 1.f)));
}

GameState::GameState(EngineData &data) : State(data)
{
    initLoadingScreen();
    initMap();
    initThisPlayer();
    initPlayerCamera();
    initPauseMenu();
    initDebugging();
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
        return;

    updateMap(dt);
    updatePlayers(dt);
    updatePlayerCamera();

    if (debugInfo)
        updateDebugText(dt);

    // Debug
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        if (keyPressedWithin(250, sf::Keyboard::Key::F1))
            debugChunks = !debugChunks;
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
    map->update(dt, thisPlayer->getCenterGridPosition());
}

void GameState::updatePlayers(const float &dt)
{
    for (auto &[addr, player] : players)
        player->update(dt, player.get() == thisPlayer.get());
}

void GameState::updatePlayerCamera()
{
    playerCamera.setCenter(thisPlayer->getCenter());

    if ((playerCamera.getCenter().x - playerCamera.getSize().x / 2.f) <= 0.f)
    {
        playerCamera.setCenter(sf::Vector2f((playerCamera.getSize().x / 2.f), playerCamera.getCenter().y));
    }
    else if ((playerCamera.getCenter().x + playerCamera.getSize().x / 2.f) >= map->getRealDimensions().x)
    {
        playerCamera.setCenter(
            sf::Vector2f((map->getRealDimensions().x - playerCamera.getSize().x / 2.f), playerCamera.getCenter().y));
    }
    if ((playerCamera.getCenter().y - playerCamera.getSize().y / 2.f) <= 0.f)
    {
        playerCamera.setCenter(sf::Vector2f(playerCamera.getCenter().x, (playerCamera.getSize().y / 2.f)));
    }
    else if ((playerCamera.getCenter().y + playerCamera.getSize().y / 2.f) >= map->getRealDimensions().y)
    {
        playerCamera.setCenter(
            sf::Vector2f(playerCamera.getCenter().x, (map->getRealDimensions().y - playerCamera.getSize().y / 2.f)));
    }
}

void GameState::updateDebugText(const float &dt)
{
    std::stringstream sstream;

    sstream << static_cast<int>(1.f / dt) << " fps" << "\n"
            << std::fixed << std::setprecision(5) << dt << " ms" << "\n"
            << "XY: " << std::fixed << std::setprecision(5) << thisPlayer->getCenter().x / data.gridSize << " | "
            << std::fixed << std::setprecision(5) << thisPlayer->getCenter().y / data.gridSize << "\n"
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
            << "]";

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

    map->render(renderTexture, thisPlayer->getCenterGridPosition(), debugChunks);

    for (auto &[name, player] : players)
        player->render(renderTexture);

    renderTexture.setView(renderTexture.getDefaultView());

    if (!pauseMenu->isActive() && debugInfo)
        renderTexture.draw(*debugText);

    pauseMenu->render(renderTexture);

    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());

    target.draw(renderSprite);
}