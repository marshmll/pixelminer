#include "stdafx.hxx"
#include "States/Options/ResourcePacksSubstate.hxx"

void ResourcePacksSubstate::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));

    sf::RectangleShape container(
        sf::Vector2f(gui::percent(data.vm->size.x, 80.f), gui::percent(data.vm->size.y, 60.f)));
    container.setPosition(
        sf::Vector2f(data.vm->size.x / 2.f - container.getSize().x / 2.f,
                     data.vm->size.y / 2.f - container.getSize().y / 2.f - gui::percent(data.vm->size.y, 5.f)));

    float gap_x = gui::percent(data.vm->size.x, 2.f);
    float gap_y = gui::percent(data.vm->size.x, 1.2f);

    int cols = 2;
    int rows = 1;

    float width = std::ceil((container.getSize().x - (cols - 1) * gap_x) / cols);
    float height = std::ceil((container.getSize().y - (rows - 1) * gap_y) / rows);

    availableContainer = std::make_unique<gui::ScrollableContainer>(
        *data.vm, sf::Vector2f(width, height), container.getPosition(), 0.f, gui::percent(data.vm->size.x, .5f),
        sf::Color(200, 200, 200, 200), sf::Color(0, 0, 0, 180));

    enabledContainer = std::make_unique<gui::ScrollableContainer>(
        *data.vm, sf::Vector2f(width, height),
        sf::Vector2f(container.getPosition().x + width + gap_x, container.getPosition().y), 0.f,
        gui::percent(data.vm->size.x, .5f), sf::Color(200, 200, 200, 200), sf::Color(0, 0, 0, 180));

    availableText = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Bold"), _("Available Resource Packs"),
                                               gui::charSize(*data.vm, 110));

    availableText->setPosition(
        sf::Vector2f(static_cast<int>(availableContainer->getPosition().x + availableContainer->getSize().x / 2.f -
                                      availableText->getGlobalBounds().size.x / 2.f),
                     static_cast<int>(availableContainer->getPosition().y - availableText->getGlobalBounds().size.y -
                                      gui::percent(data.vm->size.y, 2.f))));

    availableText->setStyle(sf::Text::Style::Underlined);

    enabledText = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Bold"), _("Enabled Resource Packs"),
                                             gui::charSize(*data.vm, 110));

    enabledText->setPosition(
        sf::Vector2f(static_cast<int>(enabledContainer->getPosition().x + enabledContainer->getSize().x / 2.f -
                                      enabledText->getGlobalBounds().size.x / 2.f),
                     static_cast<int>(enabledContainer->getPosition().y - enabledText->getGlobalBounds().size.y -
                                      gui::percent(data.vm->size.y, 2.f))));
    enabledText->setStyle(sf::Text::Style::Underlined);

    initButtons();
}

void ResourcePacksSubstate::initButtons()
{
    buttonContainer.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 70.f), gui::percent(data.vm->size.y, 13.f)));
    buttonContainer.setPosition(
        sf::Vector2f(data.vm->size.x / 2.f - buttonContainer.getSize().x / 2.f, gui::percent(data.vm->size.y, 82.f)));

    float gap_x = gui::percent(data.vm->size.x, 2.f);
    float gap_y = gui::percent(data.vm->size.x, 1.2f);

    int rows = 2;
    int cols = 2;

    float btn_width = (buttonContainer.getSize().x - (cols - 1) * gap_x) / cols;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap_y) / rows;

    buttons["OpenFolder"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                     buttonContainer.getPosition().y + (btn_height * 0) + (gap_y * 0)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Open Resource Packs Folder"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["RebuildCache"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                     buttonContainer.getPosition().y + (btn_height * 0) + (gap_y * 0)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Rebuild Cache"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["ApplyAndClose"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                     buttonContainer.getPosition().y + (btn_height * 1) + (gap_y * 1)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Apply and Close"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Back"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                     buttonContainer.getPosition().y + (btn_height * 1) + (gap_y * 1)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Back"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);
}

void ResourcePacksSubstate::initResourcePackSelectors()
{
    float padding = gui::percent(data.vm->size.x, .5f);

    for (auto &[_, resource_pack] : *data.resourcePacks)
    {
        if (resource_pack.name == data.activeResourcePack->name)
        {
            selectors.push_back(std::make_shared<ResourcePackSelector>(
                data,
                sf::Vector2f(enabledContainer->getPosition().x + padding,
                             enabledContainer->getPosition().y +
                                 (enabledResourcePackSelectors.size() *
                                  (gui::percent(data.vm->size.y, 12.f) - (padding * 2.f))) +
                                 padding),
                sf::Vector2f(enabledContainer->getSize().x - (padding * 2.f),
                             gui::percent(data.vm->size.y, 12.f) - (padding * 2.f)),
                resource_pack));

            enabledResourcePackSelectors.push_back(selectors.back());

            if (resource_pack.tag == "vanilla")
                selectors.back()->btn->setImage(data.activeResourcePack->getTexture("Forbidden"));
            else
                selectors.back()->btn->setImage(data.activeResourcePack->getTexture("ButtonArrowLeft"));
        }
        else
        {
            selectors.push_back(std::make_shared<ResourcePackSelector>(
                data,
                sf::Vector2f(availableContainer->getPosition().x + padding,
                             availableContainer->getPosition().y +
                                 (availableResourcePackSelectors.size() *
                                  (gui::percent(data.vm->size.y, 12.f) - (padding * 2.f))) +
                                 padding),
                sf::Vector2f(availableContainer->getSize().x - (padding * 2.f),
                             gui::percent(data.vm->size.y, 12.f) - (padding * 2.f)),
                resource_pack));

            availableResourcePackSelectors.push_back(selectors.back());
            selectors.back()->btn->setImage(data.activeResourcePack->getTexture("ButtonArrowRight"));
        }
    }

    if (availableResourcePackSelectors.size() > 0)
    {
        availableContainer->setMaxScrollDelta(availableResourcePackSelectors.back()->getPosition().y +
                                                  availableResourcePackSelectors.back()->getSize().y,
                                              5.f);
    }
    if (enabledResourcePackSelectors.size() > 0)
    {
        enabledContainer->setMaxScrollDelta(enabledResourcePackSelectors.back()->getPosition().y +
                                                enabledResourcePackSelectors.back()->getSize().y,
                                            5.f);
    }
}

const std::string ResourcePacksSubstate::getEnabledPacksPriorityString()
{
    std::string str;

    for (auto &selector : enabledResourcePackSelectors)
        str += selector->resourcePack.tag + ":";

    str.pop_back();

    return std::move(str);
}

ResourcePacksSubstate::ResourcePacksSubstate(EngineData &data) : Substate(data)
{
    initGUI();
    initResourcePackSelectors();
}

ResourcePacksSubstate::~ResourcePacksSubstate() = default;

void ResourcePacksSubstate::update(const float &dt)
{
    updateMousePositions();

    for (auto &[_, button] : buttons)
    {
        button->update(sf::Vector2f(mousePosWindow));

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (buttons.at("Back")->isPressed())
        killSelf();

    else if (mouseButtonPressedWithin(500, sf::Mouse::Button::Left) && buttons.at("OpenFolder")->isPressed())
        FileExplorer::browse(RESOURCES_FOLDER);

    else if (buttons.at("ApplyAndClose")->isPressed())
        std::cout << getEnabledPacksPriorityString() << "\n";

    updateAvailableSelectors(dt);
    updateEnabledSelectors(dt);
}

void ResourcePacksSubstate::updateAvailableSelectors(const float &dt)
{
    availableContainer->update(dt, mousePosView, data.mouseData);

    for (int i = 0; i < availableResourcePackSelectors.size(); i++)
    {
        availableResourcePackSelectors[i]->update(dt, sf::Vector2f(mousePosWindow));

        if (availableResourcePackSelectors[i]->isPressed())
        {
            float padding = gui::percent(data.vm->size.x, .5f);

            availableResourcePackSelectors[i]->btn->setImage(data.activeResourcePack->getTexture("ButtonArrowLeft"));

            enabledResourcePackSelectors.insert(enabledResourcePackSelectors.begin(),
                                                availableResourcePackSelectors[i]);

            for (int j = 0; j < enabledResourcePackSelectors.size(); j++)
            {
                enabledResourcePackSelectors[j]->setPosition(
                    sf::Vector2f(static_cast<int>(enabledContainer->getPosition().x + padding),
                                 static_cast<int>(enabledContainer->getPosition().y + padding +
                                                  j * (gui::percent(data.vm->size.y, 12.f)))));
            }

            enabledContainer->setMaxScrollDelta(enabledResourcePackSelectors.back()->getSize().y +
                                                    enabledResourcePackSelectors.back()->getPosition().y,
                                                5.f);

            availableResourcePackSelectors.erase(std::find(availableResourcePackSelectors.begin(),
                                                           availableResourcePackSelectors.end(),
                                                           availableResourcePackSelectors[i]));

            for (int j = 0; j < availableResourcePackSelectors.size(); j++)
            {
                availableResourcePackSelectors[j]->setPosition(
                    sf::Vector2f(static_cast<int>(availableContainer->getPosition().x + padding),
                                 static_cast<int>(availableContainer->getPosition().y + padding +
                                                  j * (gui::percent(data.vm->size.y, 12.f)))));
            }
        }
    }
}

void ResourcePacksSubstate::updateEnabledSelectors(const float &dt)
{
    enabledContainer->update(dt, mousePosView, data.mouseData);

    for (int i = 0; i < enabledResourcePackSelectors.size(); i++)
    {
        enabledResourcePackSelectors[i]->update(dt, sf::Vector2f(mousePosWindow));

        if (enabledResourcePackSelectors[i]->isPressed() &&
            enabledResourcePackSelectors[i]->resourcePack.name != "Vanilla")
        {
            float padding = gui::percent(data.vm->size.x, .5f);

            enabledResourcePackSelectors[i]->btn->setImage(data.activeResourcePack->getTexture("ButtonArrowRight"));

            availableResourcePackSelectors.insert(availableResourcePackSelectors.begin(),
                                                  enabledResourcePackSelectors[i]);

            for (int j = 0; j < availableResourcePackSelectors.size(); j++)
            {
                availableResourcePackSelectors[j]->setPosition(
                    sf::Vector2f(static_cast<int>(availableContainer->getPosition().x + padding),
                                 static_cast<int>(availableContainer->getPosition().y + padding +
                                                  j * (gui::percent(data.vm->size.y, 12.f)))));
            }

            availableContainer->setMaxScrollDelta(availableResourcePackSelectors.back()->getSize().y +
                                                      availableResourcePackSelectors.back()->getPosition().y,
                                                  5.f);

            enabledResourcePackSelectors.erase(std::find(enabledResourcePackSelectors.begin(),
                                                         enabledResourcePackSelectors.end(),
                                                         enabledResourcePackSelectors[i]));

            for (int j = 0; j < enabledResourcePackSelectors.size(); j++)
            {
                enabledResourcePackSelectors[j]->setPosition(
                    sf::Vector2f(static_cast<int>(enabledContainer->getPosition().x + padding),
                                 static_cast<int>(enabledContainer->getPosition().y + padding +
                                                  j * (gui::percent(data.vm->size.y, 12.f)))));
            }
        }
    }
}

void ResourcePacksSubstate::render(sf::RenderTarget &target)
{
    target.draw(background);

    for (auto &[_, button] : buttons)
        button->render(target);

    availableContainer->render(target);
    enabledContainer->render(target);

    target.draw(*availableText);
    target.draw(*enabledText);

    target.setView(availableContainer->getView());
    for (auto &selector : availableResourcePackSelectors)
        selector->render(target);

    target.setView(enabledContainer->getView());
    for (auto &selector : enabledResourcePackSelectors)
        selector->render(target);

    target.setView(target.getDefaultView());
}
