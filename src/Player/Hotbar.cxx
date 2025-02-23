#include "Player/Hotbar.hxx"
#include "stdafx.hxx"

void Hotbar::initGUI()
{
    maxCols = 10;
    hotbarSlot = 0;

    healthBarRows = static_cast<uint8_t>(
        std::ceil(static_cast<float>(playerAttributes.getMaxHealth()) / static_cast<float>(maxCols) / 2.f));
    hungerBarRows = static_cast<uint8_t>(
        std::ceil(static_cast<float>(playerAttributes.getMaxHunger()) / static_cast<float>(maxCols) / 2.f));

    hotbar = std::make_unique<sf::Sprite>(resourcePack->getTexture("Hotbar"));
    hotbar->setScale(sf::Vector2f(scale, scale));
    hotbar->setPosition(sf::Vector2f(vm.size.x / 2.f - hotbar->getGlobalBounds().size.x / 2.f,
                                     vm.size.y - hotbar->getGlobalBounds().size.y - gui::percent(vm.size.y, 2.f)));

    hotbarSelector = std::make_unique<sf::Sprite>(resourcePack->getTexture("HotbarSelector"));
    hotbarSelector->setScale(sf::Vector2f(scale, scale));
    updateHotbarSlotSelector();

    sf::FloatRect hotbar_bounds = hotbar->getGlobalBounds();

    sf::Sprite heart_sprite(resourcePack->getTexture("HeartFull"));
    heart_sprite.setScale(sf::Vector2f(scale, scale));
    for (int y = healthBarRows - 1; y >= 0; y--)
    {
        for (int x = 0; x < maxCols; x++)
        {
            if (healthBar.size() * 2 < playerAttributes.getMaxHealth())
            {
                heart_sprite.setPosition(
                    sf::Vector2f(hotbar_bounds.position.x + (x * heart_sprite.getGlobalBounds().size.x) + 2.f,
                                 hotbar_bounds.position.y - gui::percent(vm.size.y, 5.f) -
                                     (healthBarRows * heart_sprite.getGlobalBounds().size.y) +
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
                bread_sprite.setPosition(sf::Vector2f(hotbar_bounds.position.x + hotbar_bounds.size.x -
                                                          (maxCols * bread_sprite.getGlobalBounds().size.x) +
                                                          (x * bread_sprite.getGlobalBounds().size.x) + 2.f,
                                                      hotbar_bounds.position.y - gui::percent(vm.size.y, 5.f) -
                                                          (hungerBarRows * bread_sprite.getGlobalBounds().size.y) +
                                                          (y * bread_sprite.getGlobalBounds().size.y)));

                hungerBar.emplace_back(bread_sprite);
            }
        }
    }
}

void Hotbar::updateHotbarSlotSelector()
{
    hotbarSelector->setPosition(
        sf::Vector2f(hotbar->getPosition().x + (hotbarSlot * (hotbar->getGlobalBounds().size.y - scale * 2)) +
                         hotbar->getGlobalBounds().size.y / 2.f - hotbarSelector->getGlobalBounds().size.y / 2.f,
                     hotbar->getGlobalBounds().position.y + hotbar->getGlobalBounds().size.y / 2.f -
                         hotbarSelector->getGlobalBounds().size.y / 2.f));
}

Hotbar::Hotbar(AttributeFunctionality &player_attributes, std::shared_ptr<ResourcePack> &resorce_pack,
               sf::VideoMode &vm, const unsigned int &scale)
    : playerAttributes(player_attributes), resourcePack(resorce_pack), vm(vm), scale(scale)
{
    initGUI();
}

Hotbar::~Hotbar() = default;

void Hotbar::update(const float &dt, std::optional<sf::Event::MouseWheelScrolled> &mouse_data)
{
    updateHealthBar();
    updateHungerBar();
    updateHotbar(mouse_data);
}

void Hotbar::render(sf::RenderTarget &target)
{
    renderHealthBar(target);
    renderHungerBar(target);
    renderHotbar(target);
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

void Hotbar::updateHotbar(std::optional<sf::Event::MouseWheelScrolled> mouse_data)
{
    int8_t new_slot = hotbarSlot;

    if (mouse_data)
    {
        new_slot += mouse_data->delta;

        if (new_slot >= MAX_HOTBAR_SLOTS)
            new_slot = 0;
        else if (new_slot < 0)
            new_slot = MAX_HOTBAR_SLOTS - 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
        new_slot = 0;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
        new_slot = 1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
        new_slot = 2;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
        new_slot = 3;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5))
        new_slot = 4;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6))
        new_slot = 5;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7))
        new_slot = 6;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8))
        new_slot = 7;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9))
        new_slot = 8;

    if (new_slot != hotbarSlot)
    {
        hotbarSlot = new_slot;
        updateHotbarSlotSelector();
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

void Hotbar::renderHotbar(sf::RenderTarget &target)
{
    target.draw(*hotbar);
    target.draw(*hotbarSelector);
}
