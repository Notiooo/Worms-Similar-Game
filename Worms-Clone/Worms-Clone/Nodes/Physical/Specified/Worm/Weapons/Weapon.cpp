#include "Weapon.h"

#include <SFML/Graphics/RenderTarget.hpp>


#include "Bullet.h"

Weapon::Weapon(b2World& world, SoundPlayer& sounds, sf::Texture& weapon, sf::Texture& thumbnail, sf::Texture& bullet, const TextureManager& textures):
	weaponSprite(weapon),
	thumbnailSprite(thumbnail, sf::IntRect(0, 0, 60, 60)),
	bulletTexture(bullet),
	physicalWorld(world),
	textures(textures),
	soundPlayer(sounds)
{
	
}

void Weapon::shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force)
{
	auto bullet = std::make_unique<Bullet>(physicalWorld, soundPlayer, position, bulletTexture, textures, attackDmg, range);
	bullet->setSparkColor(bulletSparksColor);
	bullet->applyForce(force);
	rootNode->pinNode(std::move(bullet));

	if(weaponSound != Sound_ID::None)
		soundPlayer.play(weaponSound, position);
}

void Weapon::activation(Worm& worm)
{
}

void Weapon::setMaxDmg(float dmg) noexcept
{
	attackDmg = dmg;
}

void Weapon::setRange(float rng) noexcept
{
	range = rng;
}

void Weapon::setSparkColor(const sf::Color& color) noexcept
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

void Weapon::setSound(Sound_ID sound) noexcept
{
	weaponSound = sound;
}

void Weapon::rotateWeapon(float angle)
{
	weaponSprite.setOrigin(weaponSprite.getLocalBounds().width / 2.f, weaponSprite.getLocalBounds().height / 2.f);
	weaponSprite.setRotation(angle);
}

sf::Sprite& Weapon::getThumbnailSprite() noexcept
{
	return thumbnailSprite;
}
