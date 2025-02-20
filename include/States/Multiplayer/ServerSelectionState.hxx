/**
 * @file ServerSelectionState.hxx
 * @brief Declares the ServerSelectionState class that manages server connection, add, delete,
 * direct connect and other funcionalities.
 */

#pragma once

#include "GUI/GUI.hxx"
#include "States/Multiplayer/AddServerState.hxx"
#include "States/Multiplayer/DirectConnectState.hxx"
#include "States/Multiplayer/ClientGameState.hxx"
#include "States/State.hxx"

/**
 * @struct ServerMetadata
 * @brief Holds metadata information about a multiplayer server.
 */
struct ServerMetadata
{
    std::string serverName;        ///< Name of the server.
    std::string serverAddress;     ///< The address of the server.
    std::string serverDescription; ///< A short description of the server.
    std::string gameVersion;       ///< Version of the game associated with the server.
    std::string status;            ///< Current status of the server (Online/Unreachable).
    unsigned int connections;      ///< Number of players currently connected to the server.
    unsigned int maxConnections;   ///< Maximum number of players allowed on the server.
    uint64_t latency;              ///< How many microsseconds to a response from the server.
    int8_t sigStrength = 0;        ///< The quality of the connection (0 to 5).
};

/**
 * @class ServerSelector
 * @brief Represents an interactive server selection UI element.
 *
 * This class manages the visual representation and interaction logic for a server
 * selector in the multiplayer server selection menu. It includes metadata about
 * the server, UI elements like icons and text, and functionality to fetch server
 * data over the network.
 */
class ServerSelector
{
  public:
    std::mutex &mutex;                     ///< Mutex for thread-safe operations.
    sf::Texture iconTexture;               ///< Texture of the server icon.
    ServerMetadata metadata;               ///< Metadata of the associated server.
    sf::RectangleShape container;          ///< UI container for the server selector.
    sf::RectangleShape icon;               ///< Icon representing the server.
    std::unique_ptr<sf::Text> name;        ///< Displayed server name.
    std::unique_ptr<sf::Text> description; ///< Displayed server description.
    bool selected;                         ///< Indicates whether this server is currently selected.
    sf::UdpSocket &socket;                 ///< UDP socket for network communication.
    sf::SocketSelector &socketSelector;    ///< Socket selector for handling network events.
    sf::Sprite connSprite;                 ///< Sprite for connection status animation.
    Animation connAnimation;               ///< Animation for connection status.

    /**
     * @brief Constructs a ServerSelector object.
     *
     * Initializes the server selector with metadata, UI elements, and network components.
     *
     * @param mutex Reference to a mutex for thread safety.
     * @param data Reference to the engine data.
     * @param metadata Metadata of the server.
     * @param y_position Vertical position of the server selector in the menu.
     * @param socket Reference to the UDP socket for network communication.
     * @param socketSelector Reference to the socket selector for network events.
     * @param texture Texture for the connection status animation.
     */
    ServerSelector(std::mutex &mutex, EngineData &data, ServerMetadata metadata, const float y_position,
                   sf::UdpSocket &socket, sf::SocketSelector &socketSelector, sf::Texture &texture)
        : mutex(mutex), metadata(metadata), selected(false), socket(socket), socketSelector(socketSelector),
          connSprite(texture),
          connAnimation(connSprite, texture, 100, sf::Vector2u(10, 10), sf::Vector2u(1, 0), sf::Vector2u(5, 0), true)
    {
        // Initialize container
        container.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 60.f), gui::percent(data.vm->size.y, 18.f)));
        container.setPosition(sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - container.getSize().x / 2.f),
                                           static_cast<int>(y_position)));
        container.setFillColor(sf::Color(0, 0, 0, 80));
        container.setOutlineColor(sf::Color::White);

        // Initialize icon
        iconTexture = data.activeResourcePack->getTexture("Default");
        icon.setSize(sf::Vector2f(container.getSize().y, container.getSize().y));
        icon.setPosition(container.getPosition());
        icon.setTexture(&iconTexture);

        // Initialize connection sprite
        connSprite.setScale(sf::Vector2f(data.scale, data.scale));
        connSprite.setPosition(sf::Vector2f(container.getPosition().x + container.getSize().x -
                                                texture.getSize().y * data.scale - gui::percent(data.vm->size.x, 1.f),
                                            container.getPosition().y));

        // Initialize server name text
        name = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Bold"),
                                          sf::String::fromUtf8(metadata.serverName.begin(), metadata.serverName.end()),
                                          gui::charSize(*data.vm, 100));
        name->setPosition(
            sf::Vector2f(static_cast<int>(icon.getPosition().x + icon.getSize().x + gui::percent(data.vm->size.x, 1.f)),
                         static_cast<int>(icon.getPosition().y + gui::percent(data.vm->size.y, 1.f))));
        name->setFillColor(sf::Color::White);

        // Initialize server description text
        std::string str =
            metadata.serverDescription + "\n" + _("Status: ") + metadata.status + "\n" + metadata.serverAddress;
        description =
            std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"),
                                       sf::String::fromUtf8(str.begin(), str.end()), gui::charSize(*data.vm, 100));
        description->setPosition(
            sf::Vector2f(static_cast<int>(name->getPosition().x),
                         static_cast<int>(name->getPosition().y + gui::percent(data.vm->size.y, 4.f))));
        description->setFillColor(sf::Color::Yellow);
    }

    /**
     * @brief Updates the state of the server selector based on user interaction.
     *
     * @param dt Delta time for frame-based updates.
     * @param mouse_pos Current mouse position.
     */
    inline void update(const float &dt, const sf::Vector2f &mouse_pos)
    {
        if (!selected)
            container.setOutlineThickness(0.f);

        if (container.getGlobalBounds().contains(mouse_pos))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                selected = true;
                container.setOutlineThickness(1.f);
            }
        }
    }

    /**
     * @brief Renders the server selector to the target.
     *
     * @param target Render target where the server selector will be drawn.
     */
    inline void render(sf::RenderTarget &target)
    {
        if (metadata.status == _("Pending"))
            connAnimation.play();

        target.draw(container);
        target.draw(icon);
        target.draw(connSprite);
        target.draw(*name);
        target.draw(*description);
    }

    /**
     * @brief Gets the position of the server selector container.
     *
     * @return Position of the server selector container.
     */
    inline const sf::Vector2f getPosition() const
    {
        return container.getPosition();
    }

    /**
     * @brief Gets the size of the server selector container.
     *
     * @return Size of the server selector container.
     */
    inline const sf::Vector2f getSize() const
    {
        return container.getSize();
    }

    /**
     * @brief Checks if the server selector is selected.
     *
     * @return True if selected, false otherwise.
     */
    inline const bool isSelected() const
    {
        return selected;
    }

    /**
     * @brief Sets the selection state of the server selector.
     *
     * @param selected New selection state.
     */
    inline void setSelected(const bool selected)
    {
        this->selected = selected;

        if (!selected)
            container.setOutlineThickness(0.f);
        else
            container.setOutlineThickness(1.f);
    }

    /**
     * @brief Fetches server data over the network.
     *
     * Sends a request to the server and updates the metadata based on the response.
     */
    inline void fetchData()
    {
        sf::Clock clock;
        size_t colon_pos = metadata.serverAddress.find(':');
        const sf::IpAddress ipAddress = sf::IpAddress::resolve(metadata.serverAddress.substr(0, colon_pos)).value();
        const unsigned short portAddress = std::stoi(metadata.serverAddress.substr(colon_pos + 1));

        sf::Packet packet;
        packet << "INFO";

        clock.restart();

        if (socket.send(packet, ipAddress, portAddress) != sf::Socket::Status::Done)
        {
            metadata.serverDescription = _("Failed to reach server.");
            metadata.status = _("Offline");

            std::string str = metadata.serverDescription + "\n" + _("Status: ") + metadata.status + "\n" +
                              _("Game Version: ") + metadata.gameVersion;

            description->setString(sf::String::fromUtf8(str.begin(), str.end()));
            description->setFillColor(sf::Color::Red);

            connSprite.setTextureRect(sf::IntRect({0, 0}, connSprite.getTextureRect().size));

            return;
        }

        sf::Packet result;
        std::optional<sf::IpAddress> ip_buffer;
        unsigned short port_buffer;

        const float TIMEOUT = 2.f;
        const float MICROSECOND = 1000000.f;

        if (socketSelector.wait(sf::seconds(TIMEOUT)))
        {
            if (socketSelector.isReady(socket))
            {
                if (socket.receive(result, ip_buffer, port_buffer) == sf::Socket::Status::Done)
                {
                    std::string header, json;
                    result >> header >> json;

                    if (header == "ACK+INFO")
                    {
                        JObject obj = JSON::parse(json).getAs<JObject>();

                        metadata.serverName = obj.at("name").getAs<std::string>();
                        metadata.serverAddress = obj.at("address").getAs<std::string>();
                        metadata.serverDescription = obj.at("description").getAs<std::string>();
                        metadata.gameVersion = obj.at("gameVersion").getAs<std::string>();
                        metadata.status = _("Online");
                        metadata.connections = static_cast<unsigned int>(obj.at("connections").getAs<long long>());
                        metadata.maxConnections =
                            static_cast<unsigned int>(obj.at("maxConnections").getAs<long long>());
                        metadata.latency = clock.getElapsedTime().asMicroseconds();
                        metadata.sigStrength = 6 - ((metadata.latency / (TIMEOUT * MICROSECOND)) * 5);

                        name->setString(sf::String::fromUtf8(metadata.serverName.begin(), metadata.serverName.end()));

                        std::string str = metadata.serverDescription + "\n" + _("Status: ") + metadata.status + "\n" +
                                          _("Game Version: ") + metadata.gameVersion + "\n" + metadata.serverAddress;

                        description->setString(sf::String::fromUtf8(str.begin(), str.end()));
                        description->setFillColor(sf::Color::White);
                    }
                }
            }
        }

        if (metadata.sigStrength == 0)
        {
            description->setFillColor(sf::Color::Red);
            metadata.serverDescription = _("Failed to reach server.");
            metadata.status = _("Offline");

            std::string str =
                metadata.serverDescription + "\n" + _("Status: ") + metadata.status + "\n" + metadata.serverAddress;

            description->setString(sf::String::fromUtf8(str.begin(), str.end()));
        }

        connSprite.setTextureRect(sf::IntRect({metadata.sigStrength * connSprite.getTextureRect().size.x, 0},
                                              connSprite.getTextureRect().size));
    }
};

/**
 * @class ServerSelectionState
 * @brief Represents the multiplayer server selection state in the game.
 */
class ServerSelectionState : public State
{
  private:
    std::mutex mutex;

    Logger logger;

    sf::RectangleShape background;      ///< Background UI element.
    sf::RectangleShape header;          ///< Header UI element.
    sf::RectangleShape footer;          ///< Footer UI element.
    sf::RectangleShape buttonContainer; ///< Container for buttons.

    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons; ///< Map of UI buttons.

    std::unique_ptr<gui::ScrollableContainer> serverSelectorsList; ///< Scrollable list of server selectors.
    std::vector<std::shared_ptr<ServerSelector>> serverSelectors;  ///< List of available servers.
    std::optional<std::shared_ptr<ServerSelector>> selectedServer; ///< Currently selected server.

    sf::UdpSocket socket;              ///< UDP socket for communication.
    sf::SocketSelector socketSelector; ///< Selector for handling socket events.

    std::atomic_bool ready;
    std::atomic_bool threadRunning;
    std::atomic_bool abortThread;

    Server server;

    /**
     * @brief Initializes the graphical user interface elements.
     */
    void initGUI();

    void initializeButtons();

    /**
     * @brief Initializes the network socket.
     */
    void initSocket();

    /**
     * @brief Loads and initializes the list of server selectors.
     */
    void initServerSelectors();

    void loadServersFromFile();

    ServerMetadata createServerMetadata(JObject &obj);

    void addServerSelector(const ServerMetadata &metadata);

    void fetchServerInfo();

    void updateSelectorDescription(const std::shared_ptr<ServerSelector> &selector);

  public:
    /**
     * @brief Constructs the ServerSelectionState object.
     * @param data Reference to engine data.
     */
    ServerSelectionState(EngineData &data);

    /**
     * @brief Destructor for ServerSelectionState.
     */
    ~ServerSelectionState();

    /**
     * @brief Updates the multiplayer state.
     * @param dt Delta time for frame-based updates.
     */
    void update(const float &dt);

    /**
     * @brief Renders the multiplayer state.
     * @param target Render target.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Updates GUI elements.
     * @param dt Delta time for frame-based updates.
     */
    void updateGUI(const float &dt);

    void updateButtons();

    void handleButtonActions();

    void joinSelectedServer();

    void updateServerSelectors(const float &dt);

    /**
     * @brief Renders GUI elements.
     * @param target Render target.
     */
    void renderGUI(sf::RenderTarget &target);
};
