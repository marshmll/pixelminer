#include "Player/Hotbar.hxx"
#include "stdafx.hxx"

void Hotbar::initSprites()
{
    sprites["HeartEmpty"] = std::make_unique<sf::Sprite>(resorcePack->getTexture("HeartEmpty"));
    sprites["HeartHalf"] = std::make_unique<sf::Sprite>(resorcePack->getTexture("HeartHalf"));
    sprites["HeartFull"] = std::make_unique<sf::Sprite>(resorcePack->getTexture("HeartFull"));
    sprites["BreadEmpty"] = std::make_unique<sf::Sprite>(resorcePack->getTexture("BreadEmpty"));
    sprites["BreadHalf"] = std::make_unique<sf::Sprite>(resorcePack->getTexture("BreadHalf"));
    sprites["BreadFull"] = std::make_unique<sf::Sprite>(resorcePack->getTexture("BreadFull"));

    for (auto &[key, sprite] : sprites)
        sprite->setScale(sf::Vector2f(scale, scale));
}

Hotbar::Hotbar(AttributeFunctionality &player_attributes, std::shared_ptr<ResourcePack> &resorce_pack,
               const unsigned int &scale)
    : playerAttributes(player_attributes), resorcePack(resorce_pack), scale(scale)
{
    initSprites();

    maxCols = 10;
    healthBarRows = static_cast<uint8_t>(std::ceil(playerAttributes.getMaxHealth() / maxCols / 2));
    hungerBarRows = static_cast<uint8_t>(std::ceil(playerAttributes.getMaxHunger() / maxCols / 2));

    sprites.at("HeartFull")->setPosition({200.f, 200.f});
}

Hotbar::~Hotbar()
{
}

void Hotbar::update(const float &dt)
{
}

void Hotbar::render(sf::RenderTarget &target)
{
    renderHealthBar(target);
}

void Hotbar::renderHealthBar(sf::RenderTarget &target)
{
    sf::Vector2f size = sprites.at("BreadFull")->getGlobalBounds().size;

    for (int x = 0; x < maxCols; x++)
    {
        for (int y = healthBarRows - 1; y >= 0; y--)
        {
            sprites.at("BreadFull")->setPosition({200.f + size.x * x, 200.f - size.y * y});
            target.draw(*sprites.at("BreadFull"));
        }
    }
}