#include "WormHitState.h"


WormHitState::WormHitState(StateStack& stack, Worm& worm, TextureManager& textures) :
	State(stack),
	hitWorm(textures.getResourceReference(Textures_ID::HitWorm)),
	worm(worm)
{
	#ifdef _DEBUG
		worm.wormName.setString("HitState");
	#endif // DEBUG
	
	savedPosition = worm.Body->GetPosition();
}

void WormHitState::draw() const
{
}

void WormHitState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= worm.wormSprite.getTransform();
	target.draw(hitWorm, states);
}

bool WormHitState::update(sf::Time)
{
	float velocity = worm.Body->GetLinearVelocity().Length();

	if (!blocked)
	{
		// If it is not grounded then save it position
		savedPosition = worm.Body->GetPosition();

		// If it is on the ground, and moves slowly enough
		// then it can be blocked
		if (worm.footCollisions && velocity < velocityToStop)
		{
			blocked = true;
			offsetClock.restart();
		}
	}


	if(blocked)
	{
		// So if worm is grounded it stays in this position
		// and can't be moved
		worm.Body->SetTransform(savedPosition, 0);
		worm.Body->SetLinearVelocity(b2Vec2(0.f, 0.f));

		// After a second it moves to the WaitState
		if(offsetClock.getElapsedTime() > offsetTime)
			worm.activateState(State_ID::WormWaitState);
	}

	return false;
}

bool WormHitState::handleEvent(const sf::Event& event)
{
	return false;
}
