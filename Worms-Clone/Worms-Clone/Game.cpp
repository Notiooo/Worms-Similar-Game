#include "Game.h"
#include "States/States.h"
#include "States/Application_States/GameState.h"
#include "States/Application_States/TitleState.h"
#include "States/Application_States/MenuState.h"

const sf::Time Game::TIME_PER_FRAME = sf::seconds(1.f / 60.f);

Game::Game():
	gameWindow(sf::VideoMode(1280, 720), "Worms Clone")
{
	// Limit the framerate to 60 frames per second
	gameWindow.setFramerateLimit(60);

	// Setup all application-flow states
	appStack.saveState<TitleState>(State_ID::TitleState, gameWindow, fonts);
	appStack.saveState<MenuState>(State_ID::MenuState, fonts, gameWindow);
	appStack.saveState<GameState>(State_ID::GameState, gameWindow);


	// load resources
	loadResources();

	// Initial state of the statestack is TitleState
	appStack.push(State_ID::TitleState);
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
		while (frameTimeElapsed > TIME_PER_FRAME)
		{
			// Update world no more than 60 frames per seconds
			frameTimeElapsed -= TIME_PER_FRAME;
			processEvents();
			update(TIME_PER_FRAME);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (gameWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			gameWindow.close();
		appStack.handleEvent(event);
	}
}

void Game::update(sf::Time deltaTime)
{
	appStack.update(deltaTime);
}

void Game::render()
{
	// before drawing anything clean
	// the previous frame
	gameWindow.clear();

	// draw the application
	appStack.draw();
	appStack.draw(gameWindow, sf::Transform::Identity);

	// display to the window
	gameWindow.display();
}


void Game::loadResources()
{
	fonts.storeResource(Fonts_ID::ArialNarrow, "Resources/Fonts/arial_narrow.ttf");
}