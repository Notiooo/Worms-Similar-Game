#include "Game.h"

const sf::Time Game::time_per_frame = sf::seconds(1.f / 60.f);

Game::Game():
	gameWindow(sf::VideoMode(1280, 720), "Worms Clone"),
	gameWorld(gameWindow)
{
	// Limit the framerate to 60 frames per second
	gameWindow.setFramerateLimit(60);
}

void Game::run()
{
	// It controls the flow of the game loop
	// So the game is not framerate-dependent
	// so it works the same no matter what
	// performance has the player

	sf::Clock clock;
	sf::Time frameTimeElapsed = sf::Time::Zero;
	while (gameWindow.isOpen())
	{
		frameTimeElapsed += clock.restart();
		while (frameTimeElapsed > time_per_frame)
		{
			// Update world no more than 60 frames per seconds
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
		// Send all events to the game world
		// so the game world can handle them later
		gameWorld.processEvents(event);
	}
}

void Game::update(sf::Time deltaTime)
{
	// Send deltaTime to the gameWorld
	// so it can update itself with proper time
	
	// Because some for example moving object
	// works according to
	// d = st (distane = speed * time)
	gameWorld.update(deltaTime);
}

void Game::render()
{
	// First clear the window before
	// we ready to draw something
	gameWindow.clear();

	// Now draw object on the empty window
	gameWorld.draw();

	// And display it
	gameWindow.display();
}
