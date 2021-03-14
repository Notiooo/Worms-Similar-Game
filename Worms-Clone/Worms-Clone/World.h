#ifndef WORLD_H
#define WORLD_H

#include <queue>

#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "DebugBox2D.h"
#include "box2d/box2d.h"

#include "Resources/Resources.h"
#include "Nodes/NodeScene.h"
#include "SFML/Window/Event.hpp"

class Worm;

class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);

	void update(sf::Time);
	void draw() const;
	void Box2DdrawDebug();
	void processEvents(const sf::Event&);

private:
	void loadResources();
	void createWorld();
	void checkTurnTime();

	sf::RenderWindow& world_window;
	sf::View world_view;
	TextureManager world_textures; 

	sf::Time timePerTurn = sf::seconds(10); 
	sf::Time timePerHide = sf::seconds(5);

	bool hideState = false;

	std::queue<Worm*> wormQueue;

	b2World b2_World;
	DebugBox2D debugDraw;

	NodeScene root_scene;
};


#endif