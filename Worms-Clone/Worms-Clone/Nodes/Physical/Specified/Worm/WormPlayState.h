#ifndef WORMPLAYSTATE_H
#define WORMPLAYSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "WormMoveableState.h"

#include "Weapons/Weapon.h"

class WormPlayState : public WormMoveableState
{
public:
	WormPlayState(StateStack&, Worm&);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;

	// === Useful functions == //
	void shoot();

private:
	void handleShooting(const sf::Event& event);
	void updateShooting(sf::Time);

private:
	// Pointing the direction of shooting
	sf::Vector2f pointer;
	float pointerLength = 100.f;
	float pointerSpeed = 0.05f;
	float pointerAngle = b2_pi / 2.f;

	// Point the direction of shooting
	sf::CircleShape triangularPointer;

	// === Shooting Bar === //

	// Button to used to shoot
	sf::Keyboard::Key shootingKey = sf::Keyboard::Enter;
	sf::Keyboard::Key pointHigherKey = sf::Keyboard::Left;
	sf::Keyboard::Key pointLowerKey = sf::Keyboard::Right;

	// How far we can shoot
	float maxShootingForce = 35000.f;

	// It measures current shooting force load
	float currentShootingForce = 0.f;

	// It states how fast the bar is loading per second
	float shootingLoadingSpeed = (maxShootingForce * 1/2); // half bar per second

	// How long is the bear
	float shootingBarSize = 100.f;

	// Graphical element of the shooting bar
	sf::RectangleShape shootingBar;
};
#endif