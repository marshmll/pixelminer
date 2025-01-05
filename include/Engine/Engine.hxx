#pragma once

class Engine
{
private:
	sf::VideoMode vm;
	sf::RenderWindow window;
	std::optional<sf::Event> event;

	sf::Clock dtClock;
	float dt; // Delta time

	void initWindow();

	void initDeltaClock();

	void pollWindowEvents();

	void updateDeltaTime();

public:
	Engine();
	virtual ~Engine();

	void run();

	void update();

	void render();
};