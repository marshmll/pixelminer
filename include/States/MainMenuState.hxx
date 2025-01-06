#pragma once

#include "States/State.hxx"

class MainMenuState: public State
{
private:

public:
	MainMenuState(StateData &data);

	~MainMenuState();

	void update(const float &dt);

	void render(sf::RenderTarget &target);
	
};