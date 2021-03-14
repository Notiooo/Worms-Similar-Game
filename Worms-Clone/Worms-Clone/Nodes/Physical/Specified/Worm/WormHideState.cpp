#include "WormHideState.h"

void WormHideState::draw() const
{

}

WormHideState::WormHideState(StateStack& stack, Worm& worm) :
	State(stack),
	worm(worm)
{
}

void WormHideState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}

bool WormHideState::update(sf::Time)
{
	return false;
}

bool WormHideState::handleEvent(const sf::Event& event)
{
	return false;
}
