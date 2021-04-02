#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../State.h"
#include "../../World.h"

class StateStack;

/**
 * \brief The game state in which the game world is created,
 * all objects are placed and the processes inside the game world are controlled.
 */
class GameState : public State
{
public:
	GameState(StateStack& stack, sf::RenderWindow& window, int& wormAmount, int& numberOfTeams);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;

private:
	World gameWorld;
	sf::RenderWindow& gameWindow;
};


#endif