#include "WormHitState.h"


WormHitState::WormHitState(StateStack& stack, Worm& worm) :
	State(stack),
	worm(worm)
{
	#ifdef _DEBUG
		worm.wormName.setString("HitState");
	#endif // DEBUG
	//worm.Body->SetLinearVelocity(b2Vec2(0.f, 15.f));
	savedPosition = worm.Body->GetPosition();
}

void WormHitState::draw() const
{
}

void WormHitState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}

bool WormHitState::update(sf::Time)
{
	float velocity = worm.Body->GetLinearVelocity().Length();

	// If it is not grounded then save it position
	if (!worm.footCollisions || velocity > velocityToStop)
		savedPosition = worm.Body->GetPosition();


	// So if worm is grounded it stays in this position
	// and can't be moved
	if (worm.footCollisions && velocity < velocityToStop)
		worm.activateState(State_ID::WormWaitState);

	return false;
}

bool WormHitState::handleEvent(const sf::Event& event)
{
	return false;
}
