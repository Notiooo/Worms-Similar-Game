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

	void updateThis(sf::Time deltaTime) override;
	bool isDestroyed() override;

	void explode();

	virtual void collision();

	void setSparkColor(const sf::Color& color);


protected:
	float force;
	float range;

private:
	bool collided = false;
	sf::Color sparkColor = sf::Color::White;

};

#endif // !BAZOOKA_H
