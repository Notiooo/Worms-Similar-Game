#include "WormPlayState.h"
#include <memory>

#include "../../../../utils.h"

WormPlayState::WormPlayState(StateStack& stack, Worm& worm, const TextureManager& textures, SoundPlayer& sounds) :
	WormMoveableState(stack, worm, textures),
	triangularPointer(10.f, 3),
	shootingBar({shootingBarSize, 10.f}),
	soundPlayer(sounds)
{
	#ifdef SHOW_WORM_STATES
			worm.setName("PlayState");
	#endif // DEBUG

	const auto wormTeamColor = worm.teamColor;

	pointer = {std::sin(pointerAngle), std::cos(pointerAngle)};
	triangularPointer.setFillColor(wormTeamColor);
	triangularPointer.setOutlineThickness(1.5f);
	triangularPointer.setOutlineColor(sf::Color(wormTeamColor.r + 50, wormTeamColor.g + 50, wormTeamColor.b + 50,
	                                            wormTeamColor.a));
	triangularPointer.setOrigin(triangularPointer.getLocalBounds().width / 2.f,
	                            triangularPointer.getLocalBounds().height / 2.f);

	shootingBar.setOutlineThickness(1.5f);

	shootingBar.setFillColor(wormTeamColor);
	shootingBar.setOutlineColor(sf::Color(wormTeamColor.r + 50, wormTeamColor.g + 50, wormTeamColor.b + 50,
	                                      wormTeamColor.a));
	centerOrigin(shootingBar);
	shootingBar.setPosition(0, -50.f);
}


void WormPlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(triangularPointer, states);
	drawMovement(target, states);


	// Draw the shooting bar only if player is shooting (and if there is enough ammo)
	if (currentShootingForce && worm.selectedWeapon->first)
		target.draw(shootingBar, states);

	states.transform *= worm.wormSprite.getTransform();
	worm.selectedWeapon->second->rotateWeapon((pointerAngle * 180 / b2_pi) - 90);
	worm.selectedWeapon->second->drawThis(target, states);
}

bool WormPlayState::update(sf::Time deltaTime)
{
	updateMovement(deltaTime);
	updateShooting(deltaTime);
	
	return false;
}

bool WormPlayState::handleEvent(const sf::Event& event)
{
	handleMovement(event);
	handleShooting(event);


	// Controls single inputs
	switch (event.type)
	{
		// When key is released, and worm collide with the ground
		// then it velocity should be instantly reduced to zero
		case (sf::Event::KeyReleased):
		{
			if (event.key.code == sf::Keyboard::Key::E)
			{
				if (worm.footCollisions)
					worm.activateState(State_ID::WormInventoryState);
			}
		}
		break;


		// If ANY key is pressed I should
		case (sf::Event::KeyPressed):
		{

		}
		break;
	}
	return false;
}

void WormPlayState::shoot()
{
	auto& bulletsLeft = worm.selectedWeapon->first;
	auto& weapon = worm.selectedWeapon->second;
	if (bulletsLeft)
	{
		if (!weapon->isActivation())
		{
			const auto positionOfBullet = worm.getAbsolutePosition() + triangularPointer.getPosition();
			const auto directionOfBullet = sf::Vector2f(static_cast<float>(direction) * pointer.x * currentShootingForce, pointer.y * currentShootingForce);
			weapon->shoot(worm.getRootNode(), positionOfBullet, directionOfBullet);
		}
		else
			weapon->activation(worm);

		--bulletsLeft;

		if (weapon->isRoundEnding())
			worm.activateState(State_ID::WormHideState);
	}
}

void WormPlayState::handleShooting(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased && event.key.code == shootingKey)
		shoot();
}

void WormPlayState::updateShooting(sf::Time deltaTime)
{
	// Controls the pointer that shows the direction of shooting
	if (sf::Keyboard::isKeyPressed(pointHigherKey) || sf::Keyboard::isKeyPressed(pointLowerKey))
	{
		// It has to reverse the controls depending in which direction player is looking
		if ((sf::Keyboard::isKeyPressed(pointHigherKey) && !worm.facingRight()) ||
			(sf::Keyboard::isKeyPressed(pointLowerKey) && worm.facingRight()))
		{
			if (pointerAngle < b2_pi)
				pointerAngle += pointerSpeed * deltaTime.asSeconds();
		}
		else
		{
			if (pointerAngle > 0)
				pointerAngle -= pointerSpeed * deltaTime.asSeconds();
		}

		// Pointer moves in circular path
		pointer = {std::sin(pointerAngle), std::cos(pointerAngle)};
	}

	// Player has started shooting
	if (sf::Keyboard::isKeyPressed(shootingKey))
	{
		if (currentShootingForce < maxShootingForce)
			currentShootingForce += shootingLoadingSpeed * deltaTime.asSeconds();
		else
			shoot();
	}

	// The pointer showing direction of shooting has to be mirrored in case player is looking at the other side of screen
	triangularPointer.setPosition((worm.facingRight() ? -pointer.x : pointer.x) * pointerLength,
	                              pointer.y * pointerLength);

	// Set the proper size of the bar depending on current force
	shootingBar.setSize({shootingBarSize * currentShootingForce / maxShootingForce, 10.f});
}
