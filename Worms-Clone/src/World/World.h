#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/NonCopyable.hpp>
#pragma warning(push, 0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "Nodes/GameplayManager.h"
#include "Nodes/NodeScene.h"
#include "Resources/Resources.h"
#include "Sounds/SoundPlayer.h"
#include "Utils/DebugBox2D.h"
#include "World/WorldListener.h"

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


	bool isGameFinished() const;

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
	 * \brief A function that updates the dimensions of the world based
	 * on the given dimensions and position of the new object.
	 * \param position Position of the object
	 * \param dimensions Object dimensions
	 *
	 * The world dimensions then serve to block the player
	 * from escaping beyond the screen of the created game world
	 */
	void updateWorldBoundaries(sf::Vector2f position, sf::Vector2f dimensions);

	/**
	 * \brief Allows to move the camera inside the game with the mouse.
	 */
	void moveScreenWithMouse();


	// === Managers & Windows === //
	sf::RenderWindow& worldWindow; //!< Window to which game world objects are displayed
	sf::View worldView; //!< Current view of the screen
	float maxZoomFactor = 3.f; //!< Maximum zoom in/out ratio
	TextureManager worldTextures; //!< Manager containing textures used inside the game world
	FontManager worldFonts; //!< Manager containing fonts used inside the game world
	GameplayManager* worldGameManager; //!< Manager taking care of maintaining and running the game inside the game world

	// === Scene-related objects === //
	b2World b2_World; //!< Physical simulation of the game world
	DebugBox2D debugDraw; //!< Object for displaying hitboxes
	NodeScene rootScene; //!< Main stage of the game
	WorldListener worldListener; //!< It carries all collisions inside the world
	SoundPlayer worldSounds; //!< A player that allows to play sounds in the game world

	// === Properties of the game world === //
	sf::Sprite backgroundSprite; //!< Game background
	const int wormAmount; //!< Number of worms to be created per team
	const int numberOfTeams; //!< Number of teams to be created at beginning of the game
	
	float mostPositionedX = worldWindow.getSize().x; //!< The furthest object at the X axis position
	float mostPositionedY = worldWindow.getSize().y; //!< The furthest object at the Y axis position
	float lessPositionedX = 0.f; //!< Closest object at X axis position
	float lessPositionedY = 0.f; //!< Closest object at Y axis position

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
	std::array<NodeScene*, static_cast<unsigned>(WorldLayers::Counter)> worldLayers; //<! It maintain displaying in proper order
};

#endif
