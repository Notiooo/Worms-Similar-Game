#ifndef BULLET_H
#define BULLET_H

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "../../../../Physical/NodePhysicalSprite.h"
#include "../../../../../Resources/Resources.h"
#include "Weapon.h"


struct Bullet : public NodePhysicalSprite
{
	Bullet(b2World& world, sf::Vector2f position, sf::Texture& texture);
	virtual void updateThis(sf::Time deltaTime) override;
	virtual void collision();
	~Bullet();
};

#endif // !BAZOOKA_H
