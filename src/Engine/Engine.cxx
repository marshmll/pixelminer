#include "stdafx.hxx"
#include "Engine/Engine.hxx"

/* PRIVATE METHODS */

void Engine::initWindow()
{
	vm = sf::VideoMode({900, 600});
	window = sf::RenderWindow(vm, "PixelMiner", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);
}

void Engine::initVariables()
{
	dt = 0.f;
	dtClock.restart();
	gridSize = 32;
}

void Engine::initStateData()
{
	stateData.gridSize = &gridSize;
	stateData.states = &states;
}

void Engine::initMainMenuState()
{
	states.push(new MainMenuState(stateData));
}

void Engine::pollWindowEvents()
{
	while(event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
	}
}

void Engine::updateDeltaTime()
{
	dt = dtClock.restart().asSeconds();
}

/* CONSTRUCTOR / DESTRUCTOR */

Engine::Engine()
{
	initWindow();
	initVariables();
	initStateData();
	initMainMenuState();
}

Engine::~Engine()
{
	while (!states.empty())
	{
		delete states.top();
		states.pop();
	}
}

/* PUBLIC METHODS */

void Engine::run()
{
	while (window.isOpen())
	{
		pollWindowEvents();
		
		update();

		// Only render if the window is active.
		if (window.hasFocus())
			render();
	}
}

void Engine::update()
{
	updateDeltaTime();

	if (!states.empty())
	{
		if (states.top()->isDead())
			states.pop();
		else
			states.top()->update(dt);
	}
}

void Engine::render()
{
	window.clear();

	if (!states.empty())
		states.top()->render(window);

	window.display();
}
