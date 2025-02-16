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
};

/**
 * @class ServerSelectionState
 * @brief Represents the multiplayer server selection state in the game.
 */
class ServerSelectionState : public State
{
  private:
    /**
     * @class ServerSelector
     * @brief Represents an interactive server selection UI element.
     */
    class ServerSelector
    {
      public:
        EngineData &data;                      ///< Reference to engine data for accessing resources.
        sf::Texture iconTexture;               ///< Texture of the server icon.
        ServerMetadata metadata;               ///< Metadata of the associated server.
        sf::RectangleShape container;          ///< UI container for the server selector.
        sf::RectangleShape icon;               ///< Icon representing the server.
        std::unique_ptr<sf::Text> name;        ///< Displayed server name.
        std::unique_ptr<sf::Text> description; ///< Displayed server description.
        bool selected;                         ///< Indicates whether this server is currently selected.

        /**
         * @brief Constructs a ServerSelector object.
         * @param data Reference to the engine data.
         * @param metadata Server metadata.
         * @param y_position Vertical position of the server selector in the menu.
         */
        ServerSelector(EngineData &data, ServerMetadata metadata, const float y_position)
            : data(data), metadata(metadata), selected(false)
        {
            container.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 60.f), gui::percent(data.vm->size.y, 18.f)));
            container.setPosition(sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - container.getSize().x / 2.f),
                                               static_cast<int>(y_position)));
            container.setFillColor(sf::Color(0, 0, 0, 80));
            container.setOutlineColor(sf::Color::White);

            iconTexture = data.activeResourcePack->getTexture("Default");
            icon.setSize(sf::Vector2f(sf::Vector2f(container.getSize().y, container.getSize().y)));
            icon.setPosition(container.getPosition());
            icon.setTexture(&iconTexture);

            name = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Bold"), metadata.serverName,
                                              gui::charSize(*data.vm, 100));

            name->setPosition(sf::Vector2f(
                static_cast<int>(icon.getPosition().x + icon.getSize().x + gui::percent(data.vm->size.x, 1.f)),
                static_cast<int>(icon.getPosition().y + gui::percent(data.vm->size.y, 1.f))));

            name->setFillColor(sf::Color::White);

            description = std::make_unique<sf::Text>(
                data.activeResourcePack->getFont("Regular"),
                metadata.serverDescription + "\n" + _("Status: ") + metadata.status + "\n" + _("Game Version: ") +
                    metadata.gameVersion + "\n" + _("Players: ") + std::to_string(metadata.connections) + "/" +
                    std::to_string(metadata.maxConnections),
                gui::charSize(*data.vm, 100));

            description->setPosition(
                sf::Vector2f(static_cast<int>(name->getPosition().x),
                             static_cast<int>(name->getPosition().y + gui::percent(data.vm->size.y, 4.f))));

            description->setFillColor(sf::Color(200, 200, 200, 255));
        }

        /**
         * @brief Updates the state of the server selector based on user interaction.
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
         * @param target Render target where the server selector will be drawn.
         */
        inline void render(sf::RenderTarget &target)
        {
            target.draw(container);
            target.draw(icon);
            target.draw(*name);
            target.draw(*description);
        }

        /**
         * @brief Gets the position of the server selector container.
         * @return Position of the server selector container.
         */
        inline const sf::Vector2f getPosition() const
        {
            return container.getPosition();
        }

        /**
         * @brief Gets the size of the server selector container.
         * @return Size of the server selector container.
         */
        inline const sf::Vector2f getSize() const
        {
            return container.getSize();
        }

        /**
         * @brief Checks if the server selector is selected.
         * @return True if selected, false otherwise.
         */
        inline const bool isSelected() const
        {
            return selected;
        }

        /**
         * @brief Sets the selection state of the server selector.
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
    };

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

    /**
     * @brief Initializes the graphical user interface elements.
     */
    void initGUI();

    /**
     * @brief Initializes the network socket.
     */
    void initSocket();

    /**
     * @brief Loads and initializes the list of server selectors.
     */
    void initServerSelectors();

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

    /**
     * @brief Renders GUI elements.
     * @param target Render target.
     */
    void renderGUI(sf::RenderTarget &target);
};
