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
	Weapon(b2World& world, sf::Texture& weapon, sf::Texture& thumbnail, sf::Texture& bullet);

	virtual void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force);

	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;

	void rotateWeapon(float angle);
	sf::Sprite& getThumnbailSprite();


protected:
	sf::Sprite weaponSprite;
	sf::Sprite thumbnailSprite;
	sf::Texture& bulletTexture;
	b2World& physicalWorld;
};

#endif // !WEAPON_H
