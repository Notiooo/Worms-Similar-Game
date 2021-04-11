#ifndef TESTDESTRUCTIBLENODE_H
#define TESTDESTRUCTIBLENODE_H

#include <common/shapes.h>
#include <SFML/Graphics/ConvexShape.hpp>

#include "NodeScene.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../Resources/Resources.h"

class TestDestructibleNode : public NodeScene
{
public:
	TestDestructibleNode(const sf::Texture&);
	~TestDestructibleNode();

private:
	void drawThis(sf::RenderTarget&, sf::RenderStates) const override;

	sf::ConvexShape nodeShape;

	std::vector<sf::ConvexShape> shapies;
	std::vector<p2t::Point*> polyline;
	std::vector<p2t::Triangle*> triangles;
};

#endif