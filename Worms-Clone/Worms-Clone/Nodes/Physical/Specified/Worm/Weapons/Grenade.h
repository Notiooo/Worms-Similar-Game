#ifndef GRENADE_H
#define GRENADE_H

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "Weapon.h"
#include "Bullet.h"

#include "../../../../../Resources/Resources.h"

class Grenade : public Weapon
{
public:
	Grenade(b2World& world, TextureManager& textures, FontManager& fonts);
	
	void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force) override;
	
	bool isActivation() override;
	bool isRoundEnding() override;

private:
	FontManager& fonts;
};

#endif // !BAZOOKA_H
