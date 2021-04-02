#ifndef WORMMOVEABLESTATE_H
#define WORMMOVEABLESTATE_H
#include "../../../../States/State.h"
#include "Worm.h"


/**
 * \brief An abstract class that contains functions related to the movement of the worm.
 *
 * Then they inherit from it all the states that need to move inside
 * the game -- for example, HideState or PlayState
 */
class WormMoveableState : public State
{
protected:
	WormMoveableState(StateStack&, Worm&);

public:

	virtual void draw() const = 0;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
	bool update(sf::Time) override = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

	/**
	 * \brief Checks whether the worm's face turns to the right
	 * \return True if the worm looks to the right of the screen, false otherwise
	 */
	bool facingRight();


protected:
	/**
	 * \brief Interprets player input in terms of worm movement.
	 * \param event input of the player
	 *
	 * A function that is responsible for taking player input and
	 * interpreting it to create the correct movement of the worm
	 * around the map.
	 */
	void handleMovement(const sf::Event& event);

	
	/**
	 * \brief Updates the state of the worm in terms of worm movement.
	 * \param deltatime Time elapsed since the previous frame.
	 */
	void updateMovement(sf::Time deltatime);

	/**
	 * \brief Determines the direction in which the worm is looking.
	 *
	 * 1 if looks right, -1 if looks left
	 * Thanks to this I can easily set some vectors
	 * so they're pointing the proper direction
	 */
	int direction;
	
	Worm& worm;

};
#endif