#include "WormWaitState.h"


WormWaitState::WormWaitState(StateStack& stack, Worm& worm) :
	State(stack),
	worm(worm)
{
}

void WormWaitState::draw() const
{
}

void WormWaitState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}

bool WormWaitState::update(sf::Time)
{
	return false;
}

bool WormWaitState::handleEvent(const sf::Event& event)
{
	return false;
}
