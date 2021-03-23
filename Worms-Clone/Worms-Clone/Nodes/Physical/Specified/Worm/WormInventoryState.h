#ifndef WORMINVENTORYSTATE_H
#define WORMINVENTORYSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

#include "Weapons/Weapon.h"

class WormInventoryState : public State
{
public:
	WormInventoryState(StateStack&, Worm&, TextureManager& textures, sf::RenderWindow& window);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;

private:
	sf::RenderWindow& window;
	
	// Worm for which inventory should it display
	Worm& worm;

	// The background of the menu
	sf::Sprite menuSprite;

	// Rope at which the menu is swinging
	sf::Sprite ropeSprite;

	// Animating the menu
	float moveSpeed = 800.f; // how fast it moves down
	float stopPosition; // Y at which it stops
	bool stopAnimating = false;

	//
	float padding = 10.f;
	float spacing = 10.f;
	
};
#endif