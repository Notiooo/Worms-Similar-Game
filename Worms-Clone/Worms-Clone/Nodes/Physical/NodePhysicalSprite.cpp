#include "NodePhysicalSprite.h"

#include <SFML/Graphics/Texture.hpp>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

NodePhysicalSprite::NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture, sf::Vector2f position):
	NodePhysicalBody(world, physical_type, position),
	sprite(texture)
{
	centerOrigin();
	createBody();
}

NodePhysicalSprite::NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture,
	sf::Vector2f position, const sf::IntRect& rect):
	NodePhysicalBody(world, physical_type, position),
	sprite(texture)
{
	sprite.setTextureRect(rect);
	centerOrigin();
	createBody();
	
}

NodePhysicalSprite::NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture,
	sf::Vector2f position, const sf::Vector2f& size):
	NodePhysicalSprite(world, physical_type, texture, position, sf::IntRect(0, 0, size.x, size.y))
{
}

void NodePhysicalSprite::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void NodePhysicalSprite::updateThis(sf::Time deltaTime)
{
	// Synchronize the drawable Rectangle with the physical object
	updatePhysics();
}

void NodePhysicalSprite::setSize(const sf::IntRect& rect)
{
	sprite.setTextureRect(rect);
	centerOrigin();
	Body->DestroyFixture(fixture);
	createBody();
}


void NodePhysicalSprite::createBody()
{
	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox(sprite.getLocalBounds().width / 2.f / B2_SCALAR, sprite.getLocalBounds().height / 2.f / B2_SCALAR);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.2f;
	FixtureDef.shape = &Shape;
	fixture = Body->CreateFixture(&FixtureDef);
}

void NodePhysicalSprite::centerOrigin()
{
	// set origin to the center
	sf::FloatRect boundaries_of_rectangle = sprite.getLocalBounds();
	sprite.setOrigin(boundaries_of_rectangle.width / 2.f, boundaries_of_rectangle.height / 2.f);
}