#include "States/GameState.hxx"
#include "stdafx.hxx"

void GameState::initThisPlayer()
{
    players["ThisPlayer"] =
        std::make_unique<Player>(sf::Vector2f(100.f, 100.f), data.textures->at("Player1"), data.scale);
}

void GameState::initUdpListener()
{
    serverSocket.setBlocking(false);

    if (serverSocket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[ Network ] -> Error binding UDP listener to a port." << "\n";
    }

    std::cout << "[ Network ] -> UDP listener listening to port " << serverSocket.getLocalPort() << "\n";

    socketSelector.add(serverSocket);

    sf::Packet packet;

    packet << "Teste";

    if (serverSocket.send(packet, sf::IpAddress(127, 0, 0, 1), serverSocket.getLocalPort()) != sf::Socket::Status::Done)
    {
        std::cout << "[ Network ] -> Error sending the data." << "\n";
    }
}

GameState::GameState(StateData &data) : State(data)
{
    initThisPlayer();
    initUdpListener();

    debugText = std::make_unique<sf::Text>(data.fonts->at("MinecraftRegular"), "0", 16);
    debugText->setPosition({GUI::percent(data.vm->size.x, 1.f), GUI::percent(data.vm->size.y, 1.f)});

    // map = std::make_unique<Map>(sf::Vector3<unsigned int>(500, 500, 1));
}

GameState::~GameState()
{
}

void GameState::update(const float &dt)
{
    for (auto &[name, player] : players)
        player->update(dt);

    sf::Packet packet;

    packet << players.at("ThisPlayer")->getPosition().x << players.at("ThisPlayer")->getPosition().y;

    if (serverSocket.send(packet, sf::IpAddress(127, 0, 0, 1), serverSocket.getLocalPort()) != sf::Socket::Status::Done)
    {
        std::cout << "[ Network ] -> Error sending the data." << "\n";
    }

    if (socketSelector.wait(sf::milliseconds(20)))
    {
        if (socketSelector.isReady(serverSocket))
        {
            std::optional<sf::IpAddress> senderAddress;
            unsigned short senderPort;
            float x;
            float y;

            if (serverSocket.receive(packet, senderAddress, senderPort) == sf::Socket::Status::Done)
            {
                packet >> x >> y;

                // std::cout << "[ Network ] -> Some data was received from " << senderAddress->toString() << ":"
                //           << senderPort << ": \"x: " << x << ", y: " << y << "\"\n";
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