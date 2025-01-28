#include "Player/Hotbar.hxx"
#include "stdafx.hxx"

void Hotbar::initGUI()
{
    maxCols = 10;
    healthBarRows = static_cast<uint8_t>(
        std::ceil(static_cast<float>(playerAttributes.getMaxHealth()) / static_cast<float>(maxCols) / 2.f));
    hungerBarRows = static_cast<uint8_t>(
        std::ceil(static_cast<float>(playerAttributes.getMaxHunger()) / static_cast<float>(maxCols) / 2.f));

    hotbarContainer.setSize(sf::Vector2f(gui::percent(vm.size.x, 50.f), gui::percent(vm.size.y, 10.f)));
    hotbarContainer.setPosition(
        sf::Vector2f(vm.size.x / 2.f - hotbarContainer.getSize().x / 2.f, gui::percent(vm.size.y, 80.f)));

    sf::Vector2f health_icon_size = sf::Vector2f(resourcePack->getTexture("HeartFull").getSize());
    sf::Vector2f hunger_icon_size = sf::Vector2f(resourcePack->getTexture("BreadFull").getSize());

    sf::Sprite heart_sprite(resourcePack->getTexture("HeartFull"));
    heart_sprite.setScale(sf::Vector2f(scale, scale));
    for (int y = healthBarRows - 1; y >= 0; y--)
    {
        for (int x = 0; x < maxCols; x++)
        {
            if (healthBar.size() * 2 < playerAttributes.getMaxHealth())
            {
                heart_sprite.setPosition(sf::Vector2f(
                    hotbarContainer.getPosition().x + (x * heart_sprite.getGlobalBounds().size.x) + 2.f,
                    hotbarContainer.getPosition().y - (healthBarRows * heart_sprite.getGlobalBounds().size.y) +
                        (y * heart_sprite.getGlobalBounds().size.y)));

                healthBar.emplace_back(heart_sprite);
            }
        }
    }

    sf::Sprite bread_sprite(resourcePack->getTexture("BreadFull"));
    bread_sprite.setScale(sf::Vector2f(scale, scale));
    for (int y = hungerBarRows - 1; y >= 0; y--)
    {
        for (int x = maxCols - 1; x >= 0; x--)
        {
            if (hungerBar.size() * 2 < playerAttributes.getMaxHunger())
            {
                bread_sprite.setPosition(sf::Vector2f(hotbarContainer.getPosition().x + hotbarContainer.getSize().x -
                                                          (maxCols * bread_sprite.getGlobalBounds().size.x) +
                                                          (x * bread_sprite.getGlobalBounds().size.x) + 2.f,
                                                      hotbarContainer.getPosition().y -
                                                          (hungerBarRows * bread_sprite.getGlobalBounds().size.y) +
                                                          (y * bread_sprite.getGlobalBounds().size.y)));

                hungerBar.emplace_back(bread_sprite);
            }
        }
    }
}

Hotbar::Hotbar(AttributeFunctionality &player_attributes, std::shared_ptr<ResourcePack> &resorce_pack,
               sf::VideoMode &vm, const unsigned int &scale)
    : playerAttributes(player_attributes), resourcePack(resorce_pack), vm(vm), scale(scale)
{
    initGUI();
}

Hotbar::~Hotbar()
{
}

void Hotbar::update(const float &dt)
{
    updateHealthBar();
    updateHungerBar();
}

void Hotbar::render(sf::RenderTarget &target)
{
    renderHealthBar(target);
    renderHungerBar(target);
}

void Hotbar::updateHealthBar()
{
    float percent =
        static_cast<float>(playerAttributes.getHealth()) / static_cast<float>(playerAttributes.getMaxHealth());

    for (int i = 0; i < healthBar.size() * 2; i++)
    {
        float curr_percent = static_cast<float>(i) / static_cast<float>(playerAttributes.getMaxHealth());

        if (curr_percent < percent)
            healthBar[static_cast<int>(i / 2)].setTexture(resourcePack->getTexture("HeartFull"));

        else if (curr_percent == percent)
            healthBar[static_cast<int>(i / 2)].setTexture(resourcePack->getTexture("HeartHalf"));

        else
            healthBar[static_cast<int>(i / 2)].setTexture(resourcePack->getTexture("HeartEmpty"));
    }
}

void Hotbar::updateHungerBar()
{
    float percent =
        static_cast<float>(playerAttributes.getHunger()) / static_cast<float>(playerAttributes.getMaxHunger());

    for (int i = 0; i < hungerBar.size() * 2; i++)
    {
        float curr_percent = static_cast<float>(i) / static_cast<float>(playerAttributes.getMaxHunger());

        if (curr_percent < percent)
            hungerBar[static_cast<int>(i / 2)].setTexture(resourcePack->getTexture("BreadFull"));

        else if (curr_percent == percent)
            hungerBar[static_cast<int>(i / 2)].setTexture(resourcePack->getTexture("BreadHalf"));

        else
            hungerBar[static_cast<int>(i / 2)].setTexture(resourcePack->getTexture("BreadEmpty"));
    }
}

void Hotbar::renderHealthBar(sf::RenderTarget &target)
{
    for (auto &bread : hungerBar)
        target.draw(bread);
}

void Hotbar::renderHungerBar(sf::RenderTarget &target)
{
    for (auto &heart : healthBar)
        target.draw(heart);
}