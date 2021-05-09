#ifndef WORMMOVEABLESTATE_H
#define WORMMOVEABLESTATE_H
#include "../../../../States/State.h"
#include "Worm.h"
#include "../../../../Resources/Animation.h"


/**
 * \brief An abstract class that contains functions related to the movement of the worm.
 *
 * Then they inherit from it all the states that need to move inside
 * the game -- for example, HideState or PlayState
 */
class WormMoveableState : public State
{
protected:
	WormMoveableState(StateStack& stack, Worm& worm, const TextureManager& textures);

public:

	/**
	 * \brief Draws only this state (current state of the worm) to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

	/**
	 * \brief Updates the status of the worm
	 * \param deltaTime the time that has passed since the last frame.
	 */
	bool update(sf::Time deltaTime) override = 0;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	bool handleEvent(const sf::Event& event) override = 0;

	/**
	 * \brief Checks whether the worm's face turns to the right
	 * \return True if the worm looks to the right of the screen, false otherwise
	 */
	bool facingRight() const noexcept;


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
	 * \param deltaTime Time elapsed since the previous frame.
	 */
	void updateMovement(sf::Time deltaTime);


	/**
	 * \brief Draws a walking worm animation when it moves and a standard sprite when it doesn't move.
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawMovement(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * \brief Determines the direction in which the worm is looking.
	 *
	 * 1 if looks right, -1 if looks left
	 * Thanks to this I can easily set some vectors
	 * so they're pointing the proper direction
	 */
	int direction;
	
	Worm& worm;

	// === Animation === //
	Animation walkingAnimation; //!< Sprite showing animation of the worm walking
	float animationSpeedThreshold = worm.movingSpeed / 2.f; //!< Speed at which the walking animation starts

};
#endif