#include "Bullet.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "../../../CollideTypes.h"
#include <memory>


Bullet::Bullet(b2World& world, sf::Vector2f position, sf::Texture& texture):
	NodePhysicalSprite(world, NodePhysical::Physical_Types::Dynamic_Type, position, texture)
{
	// Potentially risky, 
	// actually the Body should be created here

	// This line assume there is only one fixture for this Body
	Body->GetFixtureList()[0].GetUserData().pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::Bullet, *this));
}


void Bullet::updateThis(sf::Time deltaTime)
{
	NodePhysicalSprite::updateThis(deltaTime);
	Body->SetTransform(Body->GetWorldCenter(), std::atan2(Body->GetLinearVelocity().y, Body->GetLinearVelocity().x));
}

void Bullet::collision()
{
	this->getRootNode()->unpinNode(*this);
}

Bullet::~Bullet()
{
	delete reinterpret_cast<Collision*>(Body->GetFixtureList()[0].GetUserData().pointer);
}
