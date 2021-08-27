#include "pch.h"
#include "Nodes/Physical/Specified/Worm/WormHideState.h"

WormHideState::WormHideState(StateStack& stack, Worm& worm, const TextureManager& textures) :
	WormMoveableState(stack, worm, textures)
{
	#ifdef SHOW_WORM_STATES
	worm.setName("HideState");
	#endif
}

void WormHideState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	drawMovement(target, states);
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
