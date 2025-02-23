#include "States/Options/VideoOptionsSubstate.hxx"

void VideoOptionsSubstate::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));

    buttons["Back"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(gui::percent(data.vm->size.x, 10.f), gui::percent(data.vm->size.y, 80.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 25.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Back"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);
}

VideoOptionsSubstate::VideoOptionsSubstate(EngineData &data) : Substate(data)
{
    initGUI();
}

VideoOptionsSubstate::~VideoOptionsSubstate() = default;

void VideoOptionsSubstate::update(const float &dt)
{
    updateMousePositions();

    for (auto &[_, button] : buttons)
        button->update(mousePosView);
}

void VideoOptionsSubstate::render(sf::RenderTarget &target)
{
    target.draw(background);

    for (auto &[_, button] : buttons)
        button->render(target);
}
