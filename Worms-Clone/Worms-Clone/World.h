#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <SFML/Graphics/Sprite.hpp>


#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "DebugBox2D.h"
#include "box2d/box2d.h"

#include "Resources/Resources.h"
#include "Nodes/NodeScene.h"

#include "WorldListener.h"
#include "Nodes/GameplayManager.h"


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
	World(sf::RenderWindow& window, int wormAmount, int numberOfTeams);

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
	 * \brief Allows to move the camera inside the game with the mouse.
	 */
	void moveScreenWithMouse();


	// Managers & Windows
	sf::RenderWindow& worldWindow; //!< Window to which game world objects are displayed
	sf::View worldView;
	float maxZoomFactor = 3.f;
	TextureManager worldTextures;
	FontManager worldFonts;
	GameplayManager* worldGameManager;

	// Scene-related objects
	b2World b2_World; //!< Physical simulation of the game world
	DebugBox2D debugDraw; //!< Object for displaying hitboxes
	NodeScene rootScene; //!< Main stage of the game
	WorldListener worldListener; //!< It carries all collisions inside the world
	const int wormAmount;
	const int numberOfTeams;

	/**
	 * \brief Defines individual layers on which the image will be drawn
	 */
	enum class WorldLayers
	{
		Background,
		Middle,
		Foreground,
		Counter, //!< After cast it is used to check size of the enum
	};

	std::array<NodeScene*, static_cast<unsigned>(WorldLayers::Counter)> worldLayers; //<! It helps to maintain displaying in proper order
	
	// Background
	sf::Sprite backgroundSprite; //!< Game background


	/**
	 * \brief The most commonly used variables that are passed to game objects
	 */
	struct Essentials
	{
		b2World* worldPhysical;
		sf::RenderWindow* worldWindow;
		TextureManager* worldTextures;
		FontManager* worldFonts;
	};
	Essentials essentials;
};

#endif
