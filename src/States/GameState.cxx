#include "States/GameState.hxx"
#include "stdafx.hxx"

void GameState::initThisPlayer()
{
    players[std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542)] =
        std::make_unique<Player>(sf::Vector2f(100.f, 100.f), data.textures->at("Player1"), data.scale);
}

void GameState::initUdpListener()
{
    serverSocket.setBlocking(false);

    if (serverSocket.bind(47542) != sf::Socket::Status::Done)
    {
        std::cerr << "[ Network ] -> Error binding UDP listener to a port." << "\n";
    }

    std::cout << "[ Network ] -> Listening to " << sf::IpAddress::getLocalAddress()->toString() << ":"
              << serverSocket.getLocalPort() << "\n";

    socketSelector.add(serverSocket);
}

GameState::GameState(StateData &data) : State(data)
{
    initThisPlayer();
    initUdpListener();

    debugText = std::make_unique<sf::Text>(data.fonts->at("MinecraftRegular"), "0", 16);
    debugText->setPosition(
        sf::Vector2f((int)GUI::percent(data.vm->size.x, 1.f), (int)GUI::percent(data.vm->size.y, 1.f)));

    // map = std::make_unique<Map>(sf::Vector3<unsigned int>(500, 500, 1));
}

GameState::~GameState()
{
}

void GameState::update(const float &dt)
{
    for (auto &[addr, player] : players)
        player->update(
            dt, addr == std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542));

    sf::Packet packet;

    packet << players.at(std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542))
                  ->getPosition()
                  .x
           << players.at(std::pair<sf::IpAddress, unsigned short>(sf::IpAddress::getLocalAddress().value(), 47542))
                  ->getPosition()
                  .y;

    if (serverSocket.send(packet, sf::IpAddress::Broadcast, serverSocket.getLocalPort()) != sf::Socket::Status::Done)
    {
        std::cout << "[ Network ] -> Error sending the data." << "\n";
    }

    if (socketSelector.wait(sf::milliseconds(10)))
    {
        if (socketSelector.isReady(serverSocket))
        {
            std::optional<sf::IpAddress> senderAddress;
            unsigned short senderPort;
            float x;
            float y;

            if (serverSocket.receive(packet, senderAddress, senderPort) == sf::Socket::Status::Done)
            {
                if (senderAddress.value() != sf::IpAddress::getLocalAddress().value())
                {
                    packet >> x >> y;

                    if (players.count(std::pair<sf::IpAddress, unsigned short>(senderAddress.value(), senderPort)) == 0)
                    {
                        players[std::pair<sf::IpAddress, unsigned short>(senderAddress.value(), senderPort)] =
                            std::make_unique<Player>(sf::Vector2f(x, y), data.textures->at("Player1"), data.scale);
                    }

                    players.at(std::pair<sf::IpAddress, unsigned short>(senderAddress.value(), senderPort))
                        ->setPosition({x, y});

                    // std::cout << "[ Network ] -> Some data was received from " << senderAddress->toString() << ":"
                    //           << senderPort << ": \"x: " << x << ", y: " << y << "\"\n";
                }
            }
        }
    }

    debugText->setString(std::to_string(static_cast<int>(1.f / dt)));
}

void GameState::render(sf::RenderTarget &target)
{
    for (auto &[name, player] : players)
        player->render(target);

    target.draw(*debugText);
}