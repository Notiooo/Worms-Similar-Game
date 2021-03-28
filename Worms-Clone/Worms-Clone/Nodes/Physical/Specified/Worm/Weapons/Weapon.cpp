#include "Weapon.h"

#include <SFML/Graphics/RenderTarget.hpp>


#include "Bullet.h"

Weapon::Weapon(b2World& world, sf::Texture& weapon, sf::Texture& thumbnail, sf::Texture& bullet):
	weaponSprite(weapon),
	thumbnailSprite(thumbnail, sf::IntRect(0, 0, 60, 60)),
	bulletTexture(bullet),
	physicalWorld(world)
{
	
}

void Weapon::shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(physicalWorld, position, bulletTexture, attackDmg, range);
	bullet->setSparkColor(bulletSparksColor);
	bullet->applyForce(force);
	rootNode->pinNode(std::move(bullet));
}

void Weapon::activation(Worm& worm)
{
}

void Weapon::setMaxDmg(float dmg)
{
	attackDmg = dmg;
}

void Weapon::setRange(float rng)
{
	range = rng;
}

void Weapon::setSparkColor(const sf::Color& color)
{
	bulletSparksColor = color;
}

void Weapon::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(weaponSprite, states);
}

void Weapon::updateThis(sf::Time deltaTime)
{
}

void Weapon::rotateWeapon(float angle)
{
	weaponSprite.setOrigin(weaponSprite.getLocalBounds().width / 2.f, weaponSprite.getLocalBounds().height / 2.f);
	weaponSprite.setRotation(angle);
}

sf::Sprite& Weapon::getThumbnailSprite()
{
	return thumbnailSprite;
}
