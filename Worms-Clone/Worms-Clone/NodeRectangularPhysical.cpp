#include "NodeRectangularPhysical.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

NodeRectangularPhysical::NodeRectangularPhysical(b2World& world, sf::Vector2f size, sf::Vector2f position, sf::Color color, Physical_Types physical_type):
	rectangle(size),
	physical_type(physical_type)
{
	// set origin to the center
	sf::FloatRect boundaries_of_rectangle = rectangle.getLocalBounds();
	rectangle.setOrigin(boundaries_of_rectangle.width / 2.f, boundaries_of_rectangle.height / 2.f);

	// set positon at the bottom
	rectangle.setPosition(position);

	// Set color
	rectangle.setFillColor(color);


	// Defines properties of the body
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(position.x / 30.f, position.y / 30.f);
	switch (physical_type)
	{
	case Physical_Types::Kinematic_Type:
		BodyDef.type = b2_kinematicBody; 
		break;
	case Physical_Types::Static_Type:
		BodyDef.type = b2_staticBody;
		break;
	case Physical_Types::Dynamic_Type:
		BodyDef.type = b2_dynamicBody;
		break;
	}
	Body = world.CreateBody(&BodyDef);

	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox((size.x/2.f)/30.f, (size.y/2.f)/ 30.f);
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
	rectangle.setPosition(30.f * Body->GetPosition().x, 30.f * Body->GetPosition().y);
	rectangle.setRotation(Body->GetAngle() * 180 / b2_pi);
}
