#ifndef WORLD_H
#define WORLD_H

#include <queue>
#include <SFML/Graphics/Sprite.hpp>


#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "DebugBox2D.h"
#include "box2d/box2d.h"

#include "Resources/Resources.h"
#include "Nodes/NodeScene.h"
#include "SFML/Graphics/Text.hpp"

#include "WorldListener.h"


class Worm;

/**
 * \brief The game world in which all processes inside the game world are controlled
 *
 * These are things like keeping an eye on the order of movements.
 * The turn-based game system. Displaying time on the screen,
 * inserting objects into the world.
 */
class World : sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);

	// === Functions that controls flow of the world === //

	/**
	 * \brief Updates the world logic every iteration
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	void update(sf::Time deltaTime);

	/**
	 * \brief Draws the world every frame
	 */
	void draw() const;

	/**
	 * \brief Draws all collisions (hitboxes) of physical objects to the screen
	 */
	void box2DdrawDebug();

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	void processEvents(const sf::Event& event);

private:

	// ==== Functions to create and control the game world === //
	/**
	 * \brief Loads all resources needed for the world of the game.
	 */
	void loadResources();

	/**
	 * \brief Creates and prepares the scene/world of the game
	 */
	void createWorld();

	/**
	 * \brief It controls a turn-based game system.
	 * Responsible for adhering to the rules of the game and its flow.
	 */
	void checkTurnTime();

	/**
	 * \brief Allows to move the camera inside the game with the mouse.
	 */
	void moveScreenWithMouse();

	// Variables used to control the turn-based game system.
	sf::Clock roundClock; // Timer to control game time
	sf::Time timePerTurn = sf::seconds(30); // Time allowed per turn
	sf::Time timePerHide = sf::seconds(5); // Time allowed for escape during turn change
	sf::Text roundTimeText; // Text that displays the current time on the screen
	std::deque<Worm*> wormQueue; // Order in which particular worms can play

	// Managers & Windows
	sf::RenderWindow& worldWindow;
	sf::View worldView;
	TextureManager worldTextures;
	FontManager worldFonts;

	// Scene-related objects
	b2World b2_World; // Physical simulation of the game world
	DebugBox2D debugDraw; // Object for displaying hitboxes
	NodeScene rootScene; // Main stage of the game
	WorldListener worldListener; // It carries all collisions inside the world

	// Background
	sf::Sprite backgroundSprite; // Game background
};

#endif
