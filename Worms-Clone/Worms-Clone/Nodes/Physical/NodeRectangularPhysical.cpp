#include "NodeRectangularPhysical.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

NodeRectangularPhysical::NodeRectangularPhysical(b2World& world, sf::Vector2f size, sf::Vector2f position, sf::Color color, Physical_Types physical_type):
	NodePhysical(world, physical_type, position),
	rectangle(size)
{
	// set origin to the center
	sf::FloatRect boundaries_of_rectangle = rectangle.getLocalBounds();
	rectangle.setOrigin(boundaries_of_rectangle.width / 2.f, boundaries_of_rectangle.height / 2.f);

	// set positon at the bottom
	rectangle.setPosition(position);

	// Set color
	rectangle.setFillColor(color);

	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox((size.x/2.f)/ B2_SCALAR, (size.y/2.f)/ B2_SCALAR);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.6f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);

}

void NodeRectangularPhysical::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectangle);
}

void NodeRectangularPhysical::updateThis(sf::Time deltaTime)
{
	rectangle.setPosition(B2_SCALAR * Body->GetPosition().x, B2_SCALAR * Body->GetPosition().y);
	rectangle.setRotation(Body->GetAngle() * 180 / b2_pi);
}
