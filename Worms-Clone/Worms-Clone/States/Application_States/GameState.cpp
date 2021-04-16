#include "GameState.h"

#include "../StateStack.h"
#include "SFML/Graphics/RenderWindow.hpp"

GameState::GameState(StateStack& stack, sf::RenderWindow& window, int& wormAmount, int& numberOfTeams):
	State(stack),
	gameWindow(window),
	gameWorld(window, wormAmount, numberOfTeams)
{

}


bool GameState::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestPush(State_ID::PauseState);
	
	gameWorld.processEvents(event);

	// Set this state to transparent -- in other words
	// allow States below in stack to be rendered.
	return true;
}

bool GameState::update(sf::Time deltaTime)
{
	// Send deltaTime to the gameWorld
	// so it can update itself with proper time

	// Because some for example moving object
	// works according to
	// d = st (distane = speed * time)
	gameWorld.update(deltaTime);

	if(gameWorld.isGameFinished())
	{
		gameWindow.setView(gameWindow.getDefaultView());
		requestPop();
		requestPush(State_ID::MenuState);
	}

	// Set this state to transparent -- in other words
	// allow States below in stack to be rendered.
	return true;
}

void GameState::draw() const
{
	// Now draw object on the empty window
	gameWorld.draw();
}

void GameState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}
