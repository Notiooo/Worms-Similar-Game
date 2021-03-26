#ifndef CANNON_H
#define CANNON_H

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "Weapon.h"
#include "Bullet.h"

#include "../../../../../Resources/Resources.h"

class Cannon : public Weapon
{
public:
	Cannon(b2World& world, TextureManager& textures);

	void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force) override;

	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;

private:
	float attackDmg = 150.f;
	float range = 100.f;
};

#endif // !BAZOOKA_H
