#ifndef WORM_H
#define WORM_H

#include "../NodePhysical.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../../../Resources/Resources.h"

// Listener that controls collision of the worms legs
// with the outside world. Next it increments/decrements
// footCollissions according to how many encounters
class GroundedListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};


// An object the player control
class Worm : public NodePhysical
{
public:
	friend class GroundedListener;

	Worm(b2World& world, TextureManager& textures, sf::Vector2f position);

	// Function to control flow of the worm
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;
	void handleThisEvents(const sf::Event& event) override;

	// Useful additional functions
	sf::Vector2f getSpriteSize();

private:
	sf::Sprite wormSprite;
	sf::Sprite ropeSprite;

	// Worm properties
	float jumpStrength = 300.f;
	float movingSpeed = 2.f;

	// How many collisions occur in
	// the hitbox of its "legs"
	int footCollisions = 0;

	// Controls
	sf::Keyboard::Key jumpButton = sf::Keyboard::Space;
	sf::Keyboard::Key leftButton = sf::Keyboard::A;
	sf::Keyboard::Key rightButtton = sf::Keyboard::D;

	// The listener that controls how many
	// objects collide with it foots
	GroundedListener listen_footsteps;
};

#endif