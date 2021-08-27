#include "pch.h"
#include "Nodes/Physical/NodeRectangularPhysical.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

NodeRectangularPhysical::NodeRectangularPhysical(b2World& world, sf::Vector2f size, sf::Vector2f position, sf::Color color, Physical_Types physical_type):
	NodePhysicalBody(world, physical_type, position),
	rectangle(size)
{
	// set origin to the center
	auto boundaries_of_rectangle = rectangle.getLocalBounds();
	rectangle.setOrigin(boundaries_of_rectangle.width / 2.f, boundaries_of_rectangle.height / 2.f);

	// Set color
	rectangle.setFillColor(color);

	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox(sfVectorToB2Vec(size/2.f).x, sfVectorToB2Vec(size / 2.f).y);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.2f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);

}

void NodeRectangularPhysical::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectangle, states);
}