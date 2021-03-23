#include "NodePhysicalSprite.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

NodePhysicalSprite::NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Vector2f position, sf::Texture& texture):
	NodePhysical(world, physical_type, position),
	sprite(texture)
{
	// set origin to the center
	sf::FloatRect boundaries_of_rectangle = sprite.getLocalBounds();
	sprite.setOrigin(boundaries_of_rectangle.width / 2.f, boundaries_of_rectangle.height / 2.f);

	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox(sprite.getTexture()->getSize().x / 2.f / B2_SCALAR, sprite.getTexture()->getSize().y / 2.f / B2_SCALAR);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.2f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void NodePhysicalSprite::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void NodePhysicalSprite::updateThis(sf::Time deltaTime)
{
	// Synchronize the drawable Rectangle with the physical object
	updatePhysics();
	//sprite.setPosition(b2VecToSfVector<sf::Vector2f>(Body->GetPosition()));
	//sprite.setRotation(radiansToAngle(Body->GetAngle()));
}

void NodePhysicalSprite::setRotation(float angle)
{
	// Do not change position, just change the angle
	Body->SetTransform(Body->GetPosition(), angleToRadians(angle));
}
