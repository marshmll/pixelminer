#include "Player/PlayerGUI.hxx"
#include "stdafx.hxx"

void PlayerGUI::initGUI()
{
    hotbar = std::make_unique<Hotbar>(player.getAttributeFunctionality(), resourcePack, vm, scale);

    tileHoverIndicator.setSize(sf::Vector2f(GRID_SIZE * scale, GRID_SIZE * scale));
    tileHoverIndicator.setOutlineColor(sf::Color::Black);
    tileHoverIndicator.setOutlineThickness(1.f);
    tileHoverIndicator.setTexture(&resourcePack->getTexture("TileBreaking"));
    tileHoverIndicator.setTextureRect(sf::IntRect({0 * GRID_SIZE, 0 * GRID_SIZE}, {GRID_SIZE, GRID_SIZE}));
}

PlayerGUI::PlayerGUI(Player &player, std::shared_ptr<ResourcePack> &resource_pack, sf::VideoMode &vm,
                     const unsigned int &scale)
    : player(player), resourcePack(resource_pack), vm(vm), scale(scale)
{
    initGUI();
}

PlayerGUI::~PlayerGUI() = default;

void PlayerGUI::update(const float &dt, const sf::Vector2i &mouse_pos_grid, std::optional<sf::Event::MouseWheelScrolled> &mouse_data)
{
    hotbar->update(dt, mouse_data);
    updateTileHoverIndicator(mouse_pos_grid);
}

void PlayerGUI::render(sf::RenderTarget &target)
{
    hotbar->render(target);
}

void PlayerGUI::updateTileHoverIndicator(const sf::Vector2i &mouse_pos_grid)
{
    tileHoverIndicator.setPosition(
        sf::Vector2f(mouse_pos_grid.x * GRID_SIZE * scale, mouse_pos_grid.y * GRID_SIZE * scale));

    sf::Vector2f player_grid_pos = player.getGridPosition();

    // TODO: Use a hit range instead of 4
    if (mouse_pos_grid.x < player_grid_pos.x - 3 || mouse_pos_grid.x > player_grid_pos.x + 3 ||
        mouse_pos_grid.y < player_grid_pos.y - 3 || mouse_pos_grid.y > player_grid_pos.y + 3)
        tileHoverIndicator.setOutlineThickness(0.f);
    else
        tileHoverIndicator.setOutlineThickness(1.f);
}

void PlayerGUI::renderTileHoverIndicator(sf::RenderTarget &target)
{
    target.draw(tileHoverIndicator);
}

const bool PlayerGUI::hasTileHovered() const
{
    return tileHoverIndicator.getOutlineThickness() > 0.f;
}

const sf::Vector2i PlayerGUI::getHoveredTileGridPosition()
{
    return sf::Vector2i(static_cast<int>(tileHoverIndicator.getPosition().x / GRID_SIZE / scale),
                        static_cast<int>(tileHoverIndicator.getPosition().y / GRID_SIZE / scale));
}

sf::RectangleShape &PlayerGUI::getTileHoverIndicator()
{
    return tileHoverIndicator;
}
