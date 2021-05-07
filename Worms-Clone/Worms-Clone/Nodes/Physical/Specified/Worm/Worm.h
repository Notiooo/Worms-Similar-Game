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


/**
 * \brief Worm object that can be controlled by the player
 */
class Worm : public NodePhysicalBody
{
public:
	// It may handle collisions of the worm
	friend class WorldListener;

	// States of this worm
	friend class WormHideState; //!< Player have ... seconds to hide
	friend class WormPlayState; //!< Player can play his turn
	friend class WormWaitState; //!< Player can't move with this worm
	friend class WormHitState; //!< State after getting hit by other player
	friend class WormMoveableState;
	friend class WormInventoryState; //!< State in which player can choose a weapon

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

	/**
	 * \brief Sets a new name for the worm.
	 * \param name New name of the worm
	 */
	auto setName(const std::string& name) -> void;
	
	/**
	 * \brief Returns the name of the worm.
	 * \return String with name of the worm.
	 */
	std::string getName();
	
	/**
	 * \brief A function that returns the color of the team the worm is in.
	 * \return Color of the team of the worm
	 */
	sf::Color getTeam() const;

	/**
	 * \brief Sets a new team for the worm
	 * \param teamColor Color of the new team of the worm
	 */
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
	// Uncomment if you want to see current State of
	// the worm, instead of its name
	
	// === Graphical variables === //
	sf::Sprite wormSprite; //!< Sprite being the visual representation of the worm
	sf::Sprite ropeSprite; //!< Sprite being the visual representation of the rope holding the worm
	sf::Texture& deadWorm; //!< A texture that is a visual representation of a dead worm
	sf::Text wormName; //!< Name of the worm displayed above its head
	sf::Color teamColor; //!< Team colour of this worm
	
	sf::RectangleShape healthBar; //!< Visual representation of the life bar above the worm's head
	float healthBarWidth = 60.f; //!< Length of the life bar over the head of the worm
	float healthBarHeight = 10.f; //!< Height of the life bar over the head of the worm
	

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
	float jumpStrength = 300.f; //!< The force with which the worm jumps
	float movingSpeed = 2.f; //!< The speed at which the worm moves around the map

	// Weapons
	using slot = std::pair<unsigned, std::unique_ptr<Weapon>>; //!< One place in the worm's inventory which consists of the weapon and the number of possible uses
	std::vector<slot> inventory; //!< A collection of slots that together form a grid of such items called inventory
	slot* selectedWeapon; //!< The worm's current weapon of choice

	// Health
	int health = 100; //!< Current worm life points
	int maxHealth = 100; //!< Maximum worm life points
};

#endif