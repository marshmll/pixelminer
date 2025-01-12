#include "States/GameState.hxx"
#include "stdafx.hxx"

void GameState::initMap()
{
    map = std::make_unique<Map>(*data.tileData, data.textures->at("TexturePack"), data.gridSize, data.scale,
                                56465456464654);
}

void GameState::initThisPlayer()
{
    players[std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542)] =
        std::make_shared<Player>(map->getSpawnPoint(), data.textures->at("Player1"), data.scale);

    thisPlayer = players.at(std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542));
}

void GameState::initPlayerCamera()
{
    playerCamera.setSize(sf::Vector2f(data.vm->size));
    playerCamera.setCenter(thisPlayer->getCenter());
}

void GameState::initServer()
{
    try
    {
        server.listen(55000);
        // client.connect(sf::IpAddress(127, 0, 0, 1), 55000);
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what();
    }
}

void GameState::initDebugging()
{
    debugText = std::make_unique<sf::Text>(data.fonts->at("MinecraftRegular"), "0", GUI::charSize(*data.vm, 130));
    debugText->setPosition(
        sf::Vector2f((int)GUI::percent(data.vm->size.x, 1.f), (int)GUI::percent(data.vm->size.y, 1.f)));
}

GameState::GameState(StateData &data) : State(data)
{
    initMap();
    initThisPlayer();
    initPlayerCamera();
    initServer();
    initDebugging();
}

GameState::~GameState()
{
}

void GameState::update(const float &dt)
{
    updateMap(dt);
    updatePlayers(dt);
    updatePlayerCamera();
    updateDebugText(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        client.disconnect();
}

void GameState::updateMap(const float &dt)
{
    map->update(dt);
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
        playerCamera.setCenter({playerCamera.getSize().x / 2.f, playerCamera.getCenter().y});
    }
    else if ((playerCamera.getCenter().x + playerCamera.getSize().x / 2.f) >= map->getRealDimensions().x)
    {
        playerCamera.setCenter(
            {map->getRealDimensions().x - playerCamera.getSize().x / 2.f, playerCamera.getCenter().y});
    }
    if ((playerCamera.getCenter().y - playerCamera.getSize().y / 2.f) <= 0.f)
    {
        playerCamera.setCenter({playerCamera.getCenter().x, playerCamera.getSize().y / 2.f});
    }
    else if ((playerCamera.getCenter().y + playerCamera.getSize().y / 2.f) >= map->getRealDimensions().y)
    {
        playerCamera.setCenter(
            {playerCamera.getCenter().x, map->getRealDimensions().y - playerCamera.getSize().y / 2.f});
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
    target.setView(playerCamera);

    map->render(target, false);

    for (auto &[name, player] : players)
        player->render(target);

    target.setView(target.getDefaultView());
    target.draw(*debugText);
}
