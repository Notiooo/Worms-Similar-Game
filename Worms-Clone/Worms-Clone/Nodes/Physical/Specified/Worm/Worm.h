#ifndef WORM_H
#define WORM_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../NodePhysical.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../../../../Resources/Resources.h"
#include "../../../../States/StateStack.h"
#include "SFML/Graphics/Text.hpp"
#include "Weapons/Weapon.h"
#include "SFML/Graphics/RectangleShape.hpp"

// Listener that controls collision of the worms legs
// with the outside world. Next it increments/decrements
// footCollissions according to how many encounters

// An object the player control
class Worm : public NodePhysical
{
public:
	friend class WorldListener;

	// States of this worm
	friend class WormHideState;
	friend class WormPlayState;
	friend class WormWaitState;
	friend class WormHitState;
	friend class WormMoveableState;
	friend class WormInventoryState;

	Worm(b2World& world, TextureManager& textures, FontManager& fonts, sf::RenderWindow& window, sf::Vector2f position, std::deque<Worm*>& wormQueue);

	// Function to control flow of the worm
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;
	void handleThisEvents(const sf::Event& event) override;

	void activateState(State_ID);

	
	State_ID getCurrentState() const;

	bool facingRight();

	void setDamage(int);

	void removeSelfFromQueue();

	virtual bool isDestroyed() override;



	// Useful additional functions
	sf::Vector2f getSpriteSize(const sf::Sprite&);

private:
	sf::Sprite wormSprite;
	sf::Sprite ropeSprite;
	sf::Text wormName;
	State_ID currentState;
	std::deque<Worm*>& wormQueue;

	// Worm properties
	float jumpStrength = 300.f;
	float movingSpeed = 2.f;

	// How many collisions occur in
	// the hitbox of its "legs"
	int footCollisions = 0;

	// Controls
	sf::Keyboard::Key jumpButton = sf::Keyboard::Space;
	sf::Keyboard::Key leftButton = sf::Keyboard::A;
	sf::Keyboard::Key rightButton = sf::Keyboard::D;

	// Statestack that controls flow of the states of the worm
	StateStack wormStack;

	// === Weapons === //
	using slot = std::pair<unsigned, std::unique_ptr<Weapon>>;
	std::vector<slot> inventory;
	slot* selectedWeapon;

	// === Health === //
	int health = 100;
	int maxHealth = 100;
	sf::RectangleShape healthBar;
	float healthBarWidth = 60.f;
	float healthBarHeight = 10.f;
};

#endif