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

	virtual void draw() const override;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

	virtual bool update(sf::Time) override;
	virtual bool handleEvent(const sf::Event& event) override;

	// === Useful functions == //
	void shoot();

private:
	void handleShooting(const sf::Event& event);
	void updateShooting(sf::Time);

private:
	// Pointing the direction of shooting
	sf::Vector2f pointer;
	float pointer_length = 100.f;
	float pointer_speed = 0.05f;
	float pointer_angle = b2_pi / 2.f;

	// Point the direction of shooting
	sf::CircleShape triangular_pointer;

	// === Shooting Bar === //

	// Button to used to shoot
	sf::Keyboard::Key shooting_key = sf::Keyboard::Enter;
	sf::Keyboard::Key point_higher = sf::Keyboard::Left;
	sf::Keyboard::Key point_lower = sf::Keyboard::Right;

	// How far we can shoot
	float max_shooting_force = 20000.f;

	// It measures current shooting force load
	float current_shooting_force = 0.f;

	// It states how fast the bar is loading per second
	float shooting_loading_speed = (max_shooting_force * 1/2); // half bar per second

	// How long is the bear
	float shootingBar_size = 100.f;

	// Graphical element of the shooting bar
	sf::RectangleShape shootingBar;


	// === Weapons === //
	using slot = std::pair<unsigned, std::unique_ptr<Weapon>>;
	std::vector<slot> inventory;
	slot* selected_weapon;


};
#endif