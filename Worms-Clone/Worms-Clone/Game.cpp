#include "Game.h"

const sf::Time Game::time_per_frame = sf::seconds(1.f / 60.f);

Game::Game():
	gameWindow(sf::VideoMode(640, 480), "Worms Clone"),
	gameWorld(gameWindow)
{
	gameWindow.setFramerateLimit(60);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time frameTimeElapsed = sf::Time::Zero;
	while (gameWindow.isOpen())
	{
		frameTimeElapsed += clock.restart();
		while (frameTimeElapsed > time_per_frame)
		{
			frameTimeElapsed -= time_per_frame;
			processEvents();
			update(time_per_frame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (gameWindow.pollEvent(event))
	{
		gameWorld.processEvents(event);
		// I'll do it later
	}
}

void Game::update(sf::Time deltaTime)
{
	gameWorld.update(deltaTime);
}

void Game::render()
{
	gameWindow.clear();

	gameWorld.draw();

	gameWindow.display();
}
