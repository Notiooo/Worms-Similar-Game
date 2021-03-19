#ifndef WEAPON_H
#define WEAPON_H

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "box2d/box2d.h"
#include "../../../../NodeScene.h"
#include "../../../../../Resources/Resources.h"

class Weapon : public NodeScene
{
public:
	Weapon(b2World& world, sf::Texture& weapon, sf::Texture& bullet);

	virtual void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force);

	virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateThis(sf::Time deltaTime) override;

	void collision();
	void rotateWeapon(float angle);


protected:
	sf::Sprite weaponSprite;
	sf::Texture& bulletTexture;
	b2World& physical_world;
};

#endif // !WEAPON_H
