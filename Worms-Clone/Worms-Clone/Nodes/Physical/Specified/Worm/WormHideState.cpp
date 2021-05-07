#include "WormHideState.h"

WormHideState::WormHideState(StateStack& stack, Worm& worm) :
	WormMoveableState(stack, worm)
{
	#ifdef SHOW_WORM_STATES
	worm.setName("HideState");
	#endif
}

void WormHideState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}

bool WormHideState::update(sf::Time dt)
{
	updateMovement(dt);
	return false;
}

bool WormHideState::handleEvent(const sf::Event& event)
{
	handleMovement(event);
	return false;
}
