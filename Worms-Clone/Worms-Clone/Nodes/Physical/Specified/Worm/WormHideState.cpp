#include "WormHideState.h"

void WormHideState::draw() const
{

}

WormHideState::WormHideState(StateStack& stack, Worm& worm) :
	State(stack),
	worm(worm)
{
	#ifdef _DEBUG
	worm.wormName.setString("HideState");
	#endif // DEBUG
}

void WormHideState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}

bool WormHideState::update(sf::Time)
{
	// If the Worm's legs collide with the ground
	// Then it should be possible to make a jump
	if (worm.footCollisions)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			worm.Body->SetLinearVelocity({ -worm.movingSpeed, worm.Body->GetLinearVelocity().y });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			worm.Body->SetLinearVelocity({ worm.movingSpeed, worm.Body->GetLinearVelocity().y });
	}
	return false;
}

bool WormHideState::handleEvent(const sf::Event& event)
{
	// Controls single inputs
	switch (event.type)
	{
		// When key is released, and worm collide with the ground
		// then it velocity should be instantly reduced to zero
	case (sf::Event::KeyReleased):
	{
		if (worm.footCollisions && (
			event.key.code == sf::Keyboard::A ||
			event.key.code == sf::Keyboard::D ||
			event.key.code == sf::Keyboard::W ||
			event.key.code == sf::Keyboard::S))
			worm.Body->SetLinearVelocity({ 0.f, 0.f });
		break;
	}

	// If ANY key is pressed I should
	case (sf::Event::KeyPressed):
	{
		// Rotate the Sprite in according direction
		if (event.key.code == worm.leftButton)
			worm.wormSprite.setScale(std::abs(worm.wormSprite.getScale().x), worm.wormSprite.getScale().y);

		//Rotate the Sprite in according direction
		if (event.key.code == sf::Keyboard::D)
			worm.wormSprite.setScale(-std::abs(worm.wormSprite.getScale().x), worm.wormSprite.getScale().y);

		// When Worm touches ground we should allow it to make a jump.
		if (worm.footCollisions)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				if (worm.wormSprite.getScale().x > 0)
					worm.Body->ApplyForceToCenter({ -worm.jumpStrength, -3 * worm.jumpStrength }, true);
				else
					worm.Body->ApplyForceToCenter({ worm.jumpStrength, -3 * worm.jumpStrength }, true);
			}
		}
		break;
	}

	}
	return false;
}
