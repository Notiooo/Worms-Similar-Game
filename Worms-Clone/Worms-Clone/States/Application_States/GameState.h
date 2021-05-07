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

	/**
	 * \brief Draws only this state to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the status/logic of the state
	 * \param deltaTime the time that has passed since the last frame.
	 */
	bool update(sf::Time deltaTime) override;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	bool handleEvent(const sf::Event& event) override;

private:
	World gameWorld; //!< The game world in which the whole game takes place
	sf::RenderWindow& gameWindow; //!< Window to which this status is displayed
};


#endif