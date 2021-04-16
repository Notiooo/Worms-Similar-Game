#include "WormWaitState.h"


WormWaitState::WormWaitState(StateStack& stack, Worm& worm) :
	State(stack),
	worm(worm)
{
	#ifdef SHOW_WORM_STATES
		worm.setName("WaitState");
	#endif // DEBUG
	worm.Body->SetLinearVelocity(b2Vec2(0.f, 0.f));
	savedPosition = worm.Body->GetPosition();
}

void WormWaitState::draw() const
{
}

void WormWaitState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}

bool WormWaitState::update(sf::Time)
{
	// If it is not grounded then save it position
	if (!worm.footCollisions)
		savedPosition = worm.Body->GetPosition();


	// So if worm is grounded it stays in this position
	// and can't be moved
	if (worm.footCollisions)
	{
		worm.Body->SetTransform(savedPosition, 0);
		worm.Body->SetLinearVelocity(b2Vec2(0.f, 0.f));
	}

	return false;
}

bool WormWaitState::handleEvent(const sf::Event& event)
{
	return false;
}
