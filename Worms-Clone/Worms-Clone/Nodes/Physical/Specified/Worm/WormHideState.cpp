#include "WormHideState.h"

void WormHideState::draw() const
{

}

WormHideState::WormHideState(StateStack& stack, Worm& worm) :
	WormMoveableState(stack, worm)
{
	#ifdef _DEBUG
	worm.wormName.setString("HideState");
	#endif // DEBUG
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
