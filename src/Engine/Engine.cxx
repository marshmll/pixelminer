#include "stdafx.hxx"
#include "Engine/Engine.hxx"

void Engine::initWindow()
{
	vm = sf::VideoMode({900, 600});
	window = sf::RenderWindow(vm, "PixelMiner", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);
}

void Engine::initDeltaClock()
{
	dt = 0.f;
	dtClock.restart();
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

Engine::Engine()
{
	initWindow();
	initDeltaClock();
}

Engine::~Engine()
{
}

void Engine::run()
{
	while (window.isOpen())
	{
		pollWindowEvents();
		
		update();
		render();	
	}
}

void Engine::update()
{
	updateDeltaTime();
}

void Engine::render()
{
	window.clear();

	// Render between here

	window.display();
}
