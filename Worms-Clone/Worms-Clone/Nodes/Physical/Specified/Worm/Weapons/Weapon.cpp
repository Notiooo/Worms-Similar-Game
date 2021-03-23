#include "Weapon.h"

Weapon::Weapon(b2World& world, sf::Texture& weapon, sf::Texture& thumbnail, sf::Texture& bullet):
	weaponSprite(weapon),
	thumbnailSprite(thumbnail, sf::IntRect(0, 0, 60, 60)),
	bulletTexture(bullet),
	physicalWorld(world)
{
	
}

void Weapon::shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force)
{
}

void Weapon::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void Weapon::updateThis(sf::Time deltaTime)
{
}

void Weapon::rotateWeapon(float angle)
{
	weaponSprite.setOrigin(weaponSprite.getLocalBounds().width / 2.f, weaponSprite.getLocalBounds().height / 2.f);
	weaponSprite.setRotation(angle);
}

sf::Sprite& Weapon::getThumnbailSprite()
{
	return thumbnailSprite;
}
