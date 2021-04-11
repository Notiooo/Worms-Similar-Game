#ifndef TESTDESTRUCTIBLENODE_H
#define TESTDESTRUCTIBLENODE_H

#include <clipper.hpp>
#include <common/shapes.h>
#include <SFML/Graphics/ConvexShape.hpp>

#include "NodeScene.h"
#include "Physical/NodePhysicalBody.h"

class TestDestructibleNode : public NodePhysicalBody
{
public:
	TestDestructibleNode(b2World& world, Physical_Types physical_type, sf::Vector2f position, const sf::Vector2f& size);
	~TestDestructibleNode() override;
	
	void addHole(std::vector<ClipperLib::IntPoint>& figure);
	void updateThis(sf::Time deltaTime) override;
private:
	void drawThis(sf::RenderTarget&, sf::RenderStates) const override;
	void createBody();
	b2Fixture* fixture; //!< Attaches a shape to a body

	std::vector<sf::ConvexShape> shapies;
	std::vector<p2t::Point*> polyline;
	std::vector<p2t::Triangle*> triangles;
	b2Vec2* physicalShape = nullptr;

	bool recalculateBody = false; //!< Flag checking if body should be recalculated
	sf::Vector2f originTransform;
};

#endif