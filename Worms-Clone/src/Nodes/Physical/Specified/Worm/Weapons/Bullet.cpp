#include "pch.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Bullet.h"

#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Nodes/Physical/CollideTypes.h"
#include "Nodes/Physical/NodePhysicalSpark.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Hitbox.h"


Bullet::Bullet(b2World& world, SoundPlayer& sounds, sf::Vector2f position, sf::Texture& bulletTexture, const TextureManager& textures, float force, float range):
	NodePhysicalSprite(world, Physical_Types::Dynamic_Type, bulletTexture, position),
	force(force),
	range(range),
	smokeTexture(textures.getResourceReference(Textures_ID::SmokeParticle)),
	soundPlayer(sounds)
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


bool Bullet::isDestroyed()
{
	if (NodeScene::isDestroyed())
	{
		// When destroyed, it forms particles
		// that emit an explosion
		explode();
		return true;
	}
	return false;
}

void Bullet::explode()
{
	auto hitbox = std::make_unique<Hitbox>(*World, b2VecToSfVector<sf::Vector2f>(Body->GetPosition()), range, force);
	this->getRootNode()->pinNode(std::move(hitbox));

	auto spark = std::make_unique<NodePhysicalSpark>(*World, soundPlayer, smokeTexture, b2VecToSfVector<sf::Vector2f>(Body->GetPosition()), sparkColor);
	this->getRootNode()->pinNode(std::move(spark));
}

void Bullet::collision()
{
	setDestroyed();
}

void Bullet::setSparkColor(const sf::Color& color) noexcept
{
	sparkColor = color;
}
