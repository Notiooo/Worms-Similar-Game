#ifndef WORM_H
#define WORM_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../NodePhysicalBody.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../../../../Resources/Resources.h"
#include "../../../../States/StateStack.h"
#include "SFML/Graphics/Text.hpp"
#include "Weapons/Weapon.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include <deque>


/**
 * \brief Worm object that can be controlled by the player
 */
class Worm : public NodePhysicalBody
{
public:
	// It may handle collisions of the worm
	friend class WorldListener;

	// States of this worm
	friend class WormHideState;
	friend class WormPlayState;
	friend class WormWaitState;
	friend class WormHitState;
	friend class WormMoveableState;
	friend class WormInventoryState;

	/**
	 * \brief Creates a worm 
	 * \param world The physical world in which the physical simulation of the worm is located
	 * \param textures Texture Holder where the worm texture is located
	 * \param fonts Font Holder where the font used to display worm name is located
	 * \param window Window into which the worm's inventory should be drawn
	 * \param position Position in which the worm should appear
	 * \param wormQueue Queue of movements in which the worm is placed
	 */
	Worm(b2World& world, TextureManager& textures, const FontManager& fonts, sf::RenderWindow& window, sf::Vector2f position);
	
	/**
	 * \brief Draws the worm to the given target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the logic of the worm
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	void updateThis(sf::Time deltaTime) override;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	void handleThisEvents(const sf::Event& event) override;

	/**
	 * \brief Sets the state the worm is in
	 * \param state Identifier of the state the worm should be in
	 */
	void activateState(State_ID state);

	
	/**
	 * \brief A function that checks what state the worm is currently in
	 * \return Identifier of the state the worm is in
	 */
	State_ID getCurrentState() const;

	/**
	 * \brief Checks if the worm's face is facing the right side of the screen
	 * \return True if the worm's 'sprite' is facing right, false otherwise
	 */
	bool facingRight();

	
	/**
	 * \brief Takes away a certain amount of the worm's life points
	 * \param dmg Number of health points to take away from the worm
	 */
	void setDamage(int dmg);

	void setName(const std::string& name);
	std::string getName();

	void setTeam(sf::Color teamColor);

	/**
	 * \brief Checks whether the worm is to be deleted
	 * \return True if the object should be deleted, false if not
	 */
	bool isDestroyed() override;

	/**
	 * \brief Calculates the size of the worm sprite
	 * \return Size of the worm sprite
	 */
	sf::Vector2f getWormSize() const;

private:
	//#define SHOW_WORM_STATES
	
	// === Graphical variables === //
	sf::Sprite wormSprite;
	sf::Sprite ropeSprite;
	sf::Texture& deadWorm;
	sf::Text wormName;
	sf::Color teamColor;
	
	sf::RectangleShape healthBar;
	float healthBarWidth = 60.f;
	float healthBarHeight = 10.f;
	

	// === Game flow variables === //

	StateStack wormStack; // controls flow of the states of the worm
	State_ID currentState; // holds identifier of the current state
	int footCollisions = 0; // How many collisions occur in the hitbox of its "legs"

	// === Controls === //
	sf::Keyboard::Key jumpButton = sf::Keyboard::Space;
	sf::Keyboard::Key leftButton = sf::Keyboard::A;
	sf::Keyboard::Key rightButton = sf::Keyboard::D;

	// === Worm properties === //

	// Stats
	float jumpStrength = 300.f;
	float movingSpeed = 2.f;

	// Weapons
	using slot = std::pair<unsigned, std::unique_ptr<Weapon>>;
	std::vector<slot> inventory;
	slot* selectedWeapon;

	// Health
	int health = 100;
	int maxHealth = 100;
};

#endif