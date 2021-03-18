#include "WormPlayState.h"
#include "../../NodeRectangularPhysical.h"
#include <memory>

WormPlayState::WormPlayState(StateStack& stack, Worm& worm) :
	State(stack),
	worm(worm),
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

	// 1 if looks right, -1 if looks left
	// Thanks to this I can easily set some vectors
	// so they're pointing the proper direction
	int direction = (worm.facingRight()) ? 1 : -1;
}

void WormPlayState::draw() const
{
}

void WormPlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(triangular_pointer, states);

	if(current_shooting_force)
		target.draw(shootingBar, states);
}

bool WormPlayState::update(sf::Time deltatime)
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


	// Controls the pointer that shows the direction of shooting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		// It has to reverse the controls depending in which direction player is looking
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !worm.facingRight()) ||
			(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && worm.facingRight()))
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
		if(current_shooting_force < max_shooting_force)
			current_shooting_force += shooting_loading_speed * deltatime.asSeconds();
		else
			shoot();
	}

	// The pointer showing direction of shooting has to be mirrored in case player is looking at the other side of screen
	triangular_pointer.setPosition((worm.facingRight() ? -pointer.x : pointer.x) * pointer_length, pointer.y * pointer_length);
	shootingBar.setSize({ shootingBar_size * current_shooting_force / max_shooting_force, 10.f });

	return false;
}

bool WormPlayState::handleEvent(const sf::Event& event)
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

			// Stopped shooting
			if (event.key.code == shooting_key)
				shoot();
		}
		break;


		// If ANY key is pressed I should
		case (sf::Event::KeyPressed):
		{
			// Rotate the Sprite in according direction
			if (event.key.code == worm.leftButton || event.key.code == worm.rightButtton)
			{
				// 1 if looks right, -1 if looks left
				direction = (event.key.code == worm.leftButton) ? -1 : 1;

				// By default the sprite is pointed to the left
				// so kinda misleading here is that we have to negate
				// it so it points to the right
				// Anyway notation of 1 as right is useful in vectors
				// So I can't change it here!
				worm.wormSprite.setScale(-direction * std::abs(worm.wormSprite.getScale().x),worm.wormSprite.getScale().y);
			}

			// Create test object on the world
			if (event.key.code == sf::Keyboard::X)
				shoot();

			// When Worm touches ground we should allow it to make a jump.
			if (worm.footCollisions)
			{
				if (sf::Keyboard::isKeyPressed(worm.jumpButton))
					worm.Body->ApplyForceToCenter({ direction * worm.jumpStrength, -3 * worm.jumpStrength }, true);
			}
		}

	}
	return false;
}

bool WormPlayState::facingRight()
{
	return ((direction == 1) ? true : false);
}

void WormPlayState::shoot()
{
	std::unique_ptr<NodeRectangularPhysical> box2 = std::make_unique<NodeRectangularPhysical>(*worm.World, sf::Vector2f(20, 20), worm.getAbsolutePosition() + triangular_pointer.getPosition(), sf::Color::Cyan, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	box2->applyForce({ direction * pointer.x * 10000.f, pointer.y * 10000.f });
	worm.getRootNode()->pinNode(std::move(box2));

	worm.activateHideState();
}
