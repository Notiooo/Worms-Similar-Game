#include "Bullet.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "../../../CollideTypes.h"
#include <memory>
#include "Hitbox.h"
#include "../../../NodePhysicalSpark.h"


Bullet::Bullet(b2World& world, sf::Vector2f position, sf::Texture& texture, float force, float range):
	NodePhysicalSprite(world, Physical_Types::Dynamic_Type, position, texture),
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
	// This line will calculate the position of explosion to be more accurate.
	//sf::Vector2f explosion_point = collisionPoint + sf::Vector2f(sprite.getLocalBounds().width * collisionVector.x, sprite.getLocalBounds().height * collisionVector.y);

	std::unique_ptr<Hitbox> hitbox = std::make_unique<Hitbox>(*World, b2VecToSfVector<sf::Vector2f>(Body->GetPosition()), range, force);
	this->getRootNode()->pinNode(std::move(hitbox));
	
	std::unique_ptr<NodePhysicalSpark> spark = std::make_unique<NodePhysicalSpark>(*World, b2VecToSfVector<sf::Vector2f>(Body->GetPosition()), sparkColor);
	this->getRootNode()->pinNode(std::move(spark));

}


bool Bullet::isDestroyed()
{
	if (NodeScene::isDestroyed())
	{
		collision();
		return true;
	}
	return false;
}

void Bullet::setSparkColor(const sf::Color& color)
{
	sparkColor = color;
}
