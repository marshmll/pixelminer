#pragma once

#include "States/State.hxx"

class MainMenuState: public State
{
private:
	sf::Texture bgTexture;
	sf::RectangleShape bgShape;

	void initGUI();

public:
	MainMenuState(StateData &data);

	~MainMenuState();

	void update(const float &dt);

	void render(sf::RenderTarget &target);
	
};