#ifndef WORMHIDESTATE_H
#define WORMHIDESTATE_H
#include "../../../../States/State.h"
#include "Worm.h"
#include "WormMoveableState.h"

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
	WormHideState(StateStack&, Worm&);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;
};

#endif