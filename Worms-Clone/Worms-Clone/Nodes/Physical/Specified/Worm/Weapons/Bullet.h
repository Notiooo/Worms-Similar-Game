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
	virtual void collision();
	bool isDestroyed() override final;

	void setSparkColor(const sf::Color& color);


private:
	bool collided = false;
	float force;
	float range;

	sf::Color sparkColor = sf::Color::White;

	sf::Vector2f collisionPoint;
	sf::Vector2f collisionVector;

};

#endif // !BAZOOKA_H
