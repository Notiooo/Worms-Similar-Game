#ifndef WORMHIDESTATE_H
#define WORMHIDESTATE_H

#include "Nodes/Physical/Specified/Worm/Worm.h"
#include "Nodes/Physical/Specified/Worm/WormMoveableState.h"
#include "States/State.h"

/**
 * \brief A state in which the worm has a chance to escape and hide.
 *
 * It is a rather short time. When this time elapses, the player
 * changes state to "WaitState".
 *
 * // ==== Attention / Warning!! ==== //
 * For the moment, this time change is done directly through the
 * "GameplayManager". It is possible that this will be handled here
 * in the future.
 */
class WormHideState : public WormMoveableState
{
public:
	WormHideState(StateStack& stack, Worm& worm, const TextureManager& textures);

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
};

#endif