#include "WormPlayState.h"
#include "../../NodeRectangularPhysical.h"
#include <memory>

WormPlayState::WormPlayState(StateStack& stack, Worm& worm) :
	WormMoveableState(stack, worm),
	triangularPointer(10.f, 3),
	shootingBar({ shootingBarSize, 10.f})
{
	#ifdef _DEBUG
		worm.wormName.setString("PlayState");
	#endif // DEBUG

	pointer = { std::sin(pointerAngle), std::cos(pointerAngle) };
	triangularPointer.setFillColor(sf::Color::Red);

	shootingBar.setOrigin(shootingBar.getLocalBounds().width / 2.f, shootingBar.getLocalBounds().height / 2.f);
	shootingBar.setFillColor(sf::Color::Black);
	shootingBar.setOutlineThickness(1.f);
	shootingBar.setOutlineColor(sf::Color::White);

	shootingBar.setPosition(0, -50.f);
}

void WormPlayState::draw() const
{
}

void WormPlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(triangularPointer, states);


	// Draw the shooting bar only if player is shooting
	if(currentShootingForce)
		target.draw(shootingBar, states);

	states.transform *= worm.wormSprite.getTransform();
	worm.selectedWeapon->second->rotateWeapon((pointerAngle * 180 / b2_pi) - 90);
	worm.selectedWeapon->second->drawThis(target, states);
}

bool WormPlayState::update(sf::Time deltatime)
{
	updateMovement(deltatime);
	updateShooting(deltatime);

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
				if(event.key.code == sf::Keyboard::Key::E)
				{
					if(worm.footCollisions)
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
	worm.selectedWeapon->second->shoot(worm.getRootNode(), worm.getAbsolutePosition() + triangularPointer.getPosition(), sf::Vector2f(direction * pointer.x * currentShootingForce, pointer.y * currentShootingForce));

	worm.activateState(State_ID::WormHideState);
}

void WormPlayState::handleShooting(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased && event.key.code == shootingKey)
		shoot();
}

void WormPlayState::updateShooting(sf::Time deltatime)
{
	// Controls the pointer that shows the direction of shooting
	if (sf::Keyboard::isKeyPressed(pointHigherKey) || sf::Keyboard::isKeyPressed(pointLowerKey))
	{
		// It has to reverse the controls depending in which direction player is looking
		if ((sf::Keyboard::isKeyPressed(pointHigherKey) && !worm.facingRight()) ||
			(sf::Keyboard::isKeyPressed(pointLowerKey) && worm.facingRight()))
		{
			if (pointerAngle < b2_pi)
				pointerAngle += pointerSpeed;
		}
		else
		{
			if (pointerAngle > 0)
				pointerAngle -= pointerSpeed;
		}

		// Pointer moves in circular path
		pointer = { std::sin(pointerAngle), std::cos(pointerAngle) };
	}

	// Player has started shooting
	if (sf::Keyboard::isKeyPressed(shootingKey))
	{
		if (currentShootingForce < maxShootingForce)
			currentShootingForce += shootingLoadingSpeed * deltatime.asSeconds();
		else
			shoot();
	}

	// The pointer showing direction of shooting has to be mirrored in case player is looking at the other side of screen
	triangularPointer.setPosition((worm.facingRight() ? -pointer.x : pointer.x) * pointerLength, pointer.y * pointerLength);

	// Set the proper size of the bar depending on current force
	shootingBar.setSize({ shootingBarSize * currentShootingForce / maxShootingForce, 10.f });
}


