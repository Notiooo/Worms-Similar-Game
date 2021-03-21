#ifndef BAZOOKA_H
#define BAZOOKA_H

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "Weapon.h"
#include "Bullet.h"

#include "../../../../../Resources/Resources.h"

class Bazooka : public Weapon
{
public:
	Bazooka(b2World& world, TextureManager& textures);

	virtual void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force);

	virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateThis(sf::Time deltaTime) override;

private:
	float attack_dmg = 10;
	float range = 7;
};

#endif // !BAZOOKA_H
