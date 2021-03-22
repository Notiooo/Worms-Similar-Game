#ifndef BULLET_H
#define BULLET_H

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "../../../../Physical/NodePhysicalSprite.h"
#include "../../../../../Resources/Resources.h"
#include "Weapon.h"


struct Bullet : public NodePhysicalSprite
{
	Bullet(b2World& world, sf::Vector2f position, sf::Texture& texture, float force, float range);

	virtual void updateThis(sf::Time deltaTime) override;
	virtual void collision();
	virtual void setDestroyed();
	bool isDestroyed() override final;


private:
	bool collided = false;
	float force;
	float range;

	sf::Vector2f collision_point;
	sf::Vector2f collision_vector;

};

#endif // !BAZOOKA_H
