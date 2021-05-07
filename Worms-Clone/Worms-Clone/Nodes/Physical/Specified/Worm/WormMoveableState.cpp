#include "WormMoveableState.h"

WormMoveableState::WormMoveableState(StateStack& stack, Worm& worm) :
	State(stack),
	worm(worm),
	direction(worm.Worm::facingRight() ? -1 : 1)
{
}

bool WormMoveableState::facingRight() const noexcept
{
	return ((direction == 1) ? true : false);
}

void WormMoveableState::handleMovement(const sf::Event& event)
{
	// Controls single inputs
	switch (event.type)
	{
		// When key is released, and worm collide with the ground
		// then it velocity should be instantly reduced to zero
	case (sf::Event::KeyReleased):
	{
		if (worm.footCollisions && (
			event.key.code == worm.leftButton ||
			event.key.code == worm.rightButton))
			worm.Body->SetLinearVelocity({ 0.f, 0.f });
	}
	break;


	// If ANY key is pressed I should
	case (sf::Event::KeyPressed):
	{
		// Rotate the Sprite in according direction
		if (event.key.code == worm.leftButton || event.key.code == worm.rightButton)
		{
			// 1 if looks right, -1 if looks left
			direction = (event.key.code == worm.leftButton) ? -1 : 1;

			// By default the sprite is pointed to the left
			// so kinda misleading here is that we have to negate
			// it so it points to the right
			// Anyway notation of 1 as right is useful in vectors
			// So I can't change it here!
			worm.wormSprite.setScale(-direction * std::abs(worm.wormSprite.getScale().x), worm.wormSprite.getScale().y);
		}

		// When Worm touches ground we should allow it to make a jump.
		if (worm.footCollisions)
		{
			if (event.key.code == worm.jumpButton)
			{
				worm.Body->SetLinearVelocity({ 0.f, 0.f }); // this should avoid some incredibly high jumps
				worm.Body->ApplyForceToCenter({ direction * worm.jumpStrength, -3 * worm.jumpStrength }, true);
			}
		}
	}
	break;
	}
}

void WormMoveableState::updateMovement(sf::Time deltaTime)
{
	// If the Worm's legs collide with the ground
	// Then it should be possible to make a jump
	if (worm.footCollisions)
	{
		if (sf::Keyboard::isKeyPressed(worm.leftButton))
			worm.Body->SetLinearVelocity({ -worm.movingSpeed, worm.Body->GetLinearVelocity().y });

		if (sf::Keyboard::isKeyPressed(worm.rightButton))
			worm.Body->SetLinearVelocity({ worm.movingSpeed, worm.Body->GetLinearVelocity().y });
	}
}
