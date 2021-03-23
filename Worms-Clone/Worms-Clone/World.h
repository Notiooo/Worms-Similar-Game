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

class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);

	// Functions that controls flow of the world

	// Updates the world every iteration
	void update(sf::Time);

	// Draws the world every frame
	void draw() const;

	// Draws all collisions (hitboxes) of physical objects to the screen
	void Box2DdrawDebug();

	// Process every input from the player
	void process_events(const sf::Event&);

private:
	// Loads textures & sounds & fonts
	void loadResources();

	// Insert object onto the scene
	void createWorld();

	// Controls time between the states (Round State, and
	// the gap between player moves -- Hold State)
	void check_turn_time();

	// ...
	void moveScreenWithMouse();

	// Time destined to control in-game states length
	sf::Clock roundClock;
	sf::Time timePerTurn = sf::seconds(30);
	sf::Time timePerHide = sf::seconds(5);
	// Displays timer
	sf::Text roundTimeText;

	// Managers & Windows
	sf::RenderWindow& world_window;
	sf::View world_view;
	TextureManager world_textures;
	FontManager world_fonts;

	// Scene-related objects
	b2World b2_World;
	DebugBox2D debugDraw;
	NodeScene root_scene;

	// Order in which particular worms can play
	std::deque<Worm*> wormQueue;

	// It carries all collisions inside the world
	WorldListener world_listener;

	// Background
	sf::Sprite background_sprite;
};

#endif
