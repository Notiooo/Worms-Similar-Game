#ifndef WORLD_H
#define WORLD_H

#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "box2d/box2d.h"

#include "Resources/Resources.h"
#include "Nodes/NodeScene.h"
#include "SFML/Window/Event.hpp"

class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);

	void update(sf::Time);
	void draw();
	void processEvents(const sf::Event&);

private:
	void loadResources();
	void createWorld();

	sf::RenderWindow& world_window;
	sf::View world_view;
	TextureManager world_textures; 

	b2World b2_World;

	NodeScene root_scene;
};


#endif