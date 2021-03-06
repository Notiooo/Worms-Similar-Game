#include "pch.h"
#include "States/Application_States/GameState.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "States/StateStack.h"

GameState::GameState(StateStack& stack, sf::RenderWindow& window, int& wormAmount, int& numberOfTeams, MusicPlayer& music):
	State(stack),
	gameWindow(window),
	gameWorld(window, wormAmount, numberOfTeams)
{
	music.play(Music_ID::GameState);

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
	// d = st (distance = speed * time)
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

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	gameWorld.draw();
}
