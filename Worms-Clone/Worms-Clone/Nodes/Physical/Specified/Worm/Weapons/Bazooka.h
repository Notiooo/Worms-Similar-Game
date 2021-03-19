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
	struct Bazooka_Bullet : public Bullet
	{
		Bazooka_Bullet(b2World& world, sf::Vector2f position, sf::Texture& texture);
		//virtual void updateThis(sf::Time deltaTime) override;
		virtual void collision();
	};
};

#endif // !BAZOOKA_H
