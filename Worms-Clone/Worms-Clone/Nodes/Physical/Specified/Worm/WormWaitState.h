#ifndef WORMWAITSTATE_H
#define WORMWAITSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

class WormWaitState : public State
{
public:
	WormWaitState(StateStack&, Worm&);

	/**
	 * \brief Draws only this state.
	 */
	void draw() const override;

	/**
	 * \brief Draws only this state (current state of the worm) to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the status of the worm
	 * \param deltaTime the time that has passed since the last frame.
	 */
	bool update(sf::Time deltaTime) override;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	bool handleEvent(const sf::Event& event) override;

private:
	Worm& worm;
	
	/**
	 * \brief Current position of the player.
	 *
	 * The saved is used to block the movement
	 * of the character so that no physical object
	 * can move it.
	 */
	b2Vec2 savedPosition;
};

#endif