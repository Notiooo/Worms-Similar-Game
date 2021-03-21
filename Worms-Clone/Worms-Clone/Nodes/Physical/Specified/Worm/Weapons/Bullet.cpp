#include "Bullet.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "../../../CollideTypes.h"
#include <memory>
#include "Hitbox.h"


Bullet::Bullet(b2World& world, sf::Vector2f position, sf::Texture& texture, float force, float range):
	NodePhysicalSprite(world, NodePhysical::Physical_Types::Dynamic_Type, position, texture),
	force(force),
	range(range)
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
	std::unique_ptr<Hitbox> hitbox = std::make_unique<Hitbox>(*World, b2Vec_to_sfVector<sf::Vector2f>(Body->GetPosition()), range, force);
	this->getRootNode()->pinNode(std::move(hitbox));

}

void Bullet::setDestroyed()
{
	collided = true;
}

bool Bullet::isDestroyed()
{
	if (collided)
	{
		collision();
		return true;
	}
	return false;
}
