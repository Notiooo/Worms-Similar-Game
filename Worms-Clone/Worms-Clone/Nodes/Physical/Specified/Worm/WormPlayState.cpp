#include "WormPlayState.h"
#include "../../NodeRectangularPhysical.h"
#include <memory>

WormPlayState::WormPlayState(StateStack& stack, Worm& worm) :
	WormMoveableState(stack, worm),
	triangular_pointer(10.f, 3),
	shootingBar({ shootingBar_size, 10.f})
{
	#ifdef _DEBUG
		worm.wormName.setString("PlayState");
	#endif // DEBUG

	pointer = { std::sin(pointer_angle), std::cos(pointer_angle) };
	triangular_pointer.setFillColor(sf::Color::Red);

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
	target.draw(triangular_pointer, states);


	// Draw the shooting bar only if player is shooting
	if(current_shooting_force)
		target.draw(shootingBar, states);

	states.transform *= worm.wormSprite.getTransform();
	worm.selected_weapon->second->rotateWeapon((pointer_angle * 180 / b2_pi) - 90);
	worm.selected_weapon->second->drawThis(target, states);
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
	worm.selected_weapon->second->shoot(worm.getRootNode(), worm.getAbsolutePosition() + triangular_pointer.getPosition(), sf::Vector2f(direction * pointer.x * current_shooting_force, pointer.y * current_shooting_force));

	worm.activateHideState();
}

void WormPlayState::handleShooting(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased && event.key.code == shooting_key)
		shoot();
}

void WormPlayState::updateShooting(sf::Time deltatime)
{
	// Controls the pointer that shows the direction of shooting
	if (sf::Keyboard::isKeyPressed(point_higher) || sf::Keyboard::isKeyPressed(point_lower))
	{
		// It has to reverse the controls depending in which direction player is looking
		if ((sf::Keyboard::isKeyPressed(point_higher) && !worm.facingRight()) ||
			(sf::Keyboard::isKeyPressed(point_lower) && worm.facingRight()))
		{
			if (pointer_angle < b2_pi)
				pointer_angle += pointer_speed;
		}
		else
		{
			if (pointer_angle > 0)
				pointer_angle -= pointer_speed;
		}

		// Pointer moves in circular path
		pointer = { std::sin(pointer_angle), std::cos(pointer_angle) };
	}

	// Player has started shooting
	if (sf::Keyboard::isKeyPressed(shooting_key))
	{
		if (current_shooting_force < max_shooting_force)
			current_shooting_force += shooting_loading_speed * deltatime.asSeconds();
		else
			shoot();
	}

	// The pointer showing direction of shooting has to be mirrored in case player is looking at the other side of screen
	triangular_pointer.setPosition((worm.facingRight() ? -pointer.x : pointer.x) * pointer_length, pointer.y * pointer_length);

	// Set the proper size of the bar depending on current force
	shootingBar.setSize({ shootingBar_size * current_shooting_force / max_shooting_force, 10.f });
}


