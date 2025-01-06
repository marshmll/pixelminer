#include "stdafx.hxx"
#include "States/MainMenuState.hxx"

MainMenuState::MainMenuState(StateData &data): State(data) { }

MainMenuState::~MainMenuState() { }

void MainMenuState::update(const float &dt)
{

}

void MainMenuState::render(sf::RenderTarget &target)
{
	// TEST ONLY!
	sf::RectangleShape rect;
	rect.setPosition({200.f, 200.f});
	rect.setSize({100.f, 100.f});

	target.draw(rect);
}