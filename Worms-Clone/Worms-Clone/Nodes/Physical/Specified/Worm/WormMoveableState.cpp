#include "WormMoveableState.h"

WormMoveableState::WormMoveableState(StateStack& stack, Worm& worm, const TextureManager& textures) :
	State(stack),
	worm(worm),
	direction(worm.Worm::facingRight() ? -1 : 1),
	walkingAnimation(textures.getResourceReference(Textures_ID::WormWalking), sf::Vector2i(45, 49), 8, sf::seconds(1)),
	walkingSoundTime(worm.soundPlayer.getSoundLength(soundOfWalking))
{
	walkingAnimation.setReversing(true);
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

		// If the player starts moving then the animation should start from the beginning.
		if (event.key.code == worm.leftButton || event.key.code == worm.rightButton)
		{
			walkingAnimation.restartAnimation();
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

		// It check if the worm is moving at a high enough speed to trigger its moving sound
		if (direction * worm.Body->GetLinearVelocity().x > animationSpeedThreshold && soundTimeElapsed.getElapsedTime() > walkingSoundTime)
		{
			worm.soundPlayer.play(soundOfWalking, worm.getAbsolutePosition());
			soundTimeElapsed.restart();
		}
	}

	// Update the walking animation
	walkingAnimation.update(deltaTime);
}

void WormMoveableState::drawMovement(sf::RenderTarget& target, sf::RenderStates states) const
{
	// If the worm does not move at a certain speed or touch the ground, a normal worm is drawn
	if (!(direction * worm.Body->GetLinearVelocity().x > animationSpeedThreshold && worm.footCollisions))
	{
		target.draw(worm.wormSprite, states);
	}
	// Otherwise, an animated sprite is drawn containing animation of the worm walking
	else
	{
		states.transform *= worm.wormSprite.getTransform();
		target.draw(walkingAnimation, states);
	}
}
