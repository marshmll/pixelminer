#include "States/GameState.hxx"
#include "stdafx.hxx"

void GameState::initThisPlayer()
{
    players[std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542)] =
        std::make_shared<Player>(sf::Vector2f(100.f, 100.f), data.textures->at("Player1"), data.scale);

    thisPlayer = players.at(std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542));
}

void GameState::initPlayerCamera()
{
    playerCamera.setSize(sf::Vector2f(data.vm->size));
    playerCamera.setCenter(thisPlayer->getCenter());
}

void GameState::initMap()
{
    map = std::make_unique<Map>(sf::Vector2u(2, 2), *data.tileData, data.textures->at("TexturePack"), data.gridSize,
                                data.scale);
}

void GameState::initUdpSocket()
{
    udpSocket.setBlocking(false);

    if (udpSocket.bind(47542) != sf::Socket::Status::Done)
    {
        std::cerr << "[ Network ] -> Error binding UDP listener to a port." << "\n";
    }

    std::cout << "[ Network ] -> Listening to " << sf::IpAddress::getLocalAddress()->toString() << ":"
              << udpSocket.getLocalPort() << "\n";

    socketSelector.add(udpSocket);
}

void GameState::initNetworkThreads(const unsigned short port)
{
    std::thread(&GameState::receiveGameStateThread, this).detach();
    std::thread(&GameState::broadcastGameStateThread, this).detach();
}

void GameState::initDebugging()
{
    debugText = std::make_unique<sf::Text>(data.fonts->at("MinecraftRegular"), "0", GUI::charSize(*data.vm, 120));
    debugText->setPosition(
        sf::Vector2f((int)GUI::percent(data.vm->size.x, 1.f), (int)GUI::percent(data.vm->size.y, 1.f)));
}

void GameState::broadcastGameStateThread()
{
    using namespace std::chrono_literals;

    while (!isDead())
    {
        sf::Packet packet;

        packet << thisPlayer->getPosition().x << thisPlayer->getPosition().y;

        if (udpSocket.send(packet, sf::IpAddress::Broadcast, udpSocket.getLocalPort()) != sf::Socket::Status::Done)
        {
            std::cout << "[ Network ] -> Error sending the data." << "\n";
        }

        std::this_thread::sleep_for(30ms);
    }
}

void GameState::receiveGameStateThread()
{
    while (!isDead())
    {
        if (socketSelector.wait(sf::seconds(5.f)))
        {
            if (socketSelector.isReady(udpSocket))
            {
                sf::Packet packet;
                std::optional<sf::IpAddress> senderAddress;
                unsigned short senderPort;
                float x;
                float y;

                if (udpSocket.receive(packet, senderAddress, senderPort) == sf::Socket::Status::Done)
                {
                    if (senderAddress.value() != sf::IpAddress::getLocalAddress().value())
                    {
                        packet >> x >> y;

                        if (players.count(
                                std::pair<sf::IpAddress, unsigned short>(senderAddress.value(), senderPort)) == 0)
                        {
                            players[std::pair<sf::IpAddress, unsigned short>(senderAddress.value(), senderPort)] =
                                std::make_shared<Player>(sf::Vector2f(x, y), data.textures->at("Player1"), data.scale);
                        }

                        players.at(std::pair<sf::IpAddress, unsigned short>(senderAddress.value(), senderPort))
                            ->setPosition({x, y});
                    }
                }
            }
        }
    }
}

GameState::GameState(StateData &data) : State(data)
{
    initThisPlayer();
    initPlayerCamera();
    initMap();
    // initUdpSocket();
    // initNetworkThreads(0);
    initDebugging();
}

GameState::~GameState()
{
}

void GameState::update(const float &dt)
{
    playerCamera.setCenter(thisPlayer->getCenter());

    map->update(dt);

    for (auto &[addr, player] : players)
        player->update(dt, player.get() == thisPlayer.get());

    debugText->setString(std::to_string(static_cast<int>(1.f / dt)));
}

void GameState::render(sf::RenderTarget &target)
{
    target.setView(playerCamera);

    map->render(target);

    for (auto &[name, player] : players)
        player->render(target);

    target.setView(target.getDefaultView());
    target.draw(*debugText);
}