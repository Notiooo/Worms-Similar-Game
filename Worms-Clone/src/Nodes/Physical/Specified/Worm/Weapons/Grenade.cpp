#include "pch.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Grenade.h"

#include "Nodes/GameplayManager.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Delayed_Bullet.h"


Grenade::Grenade(b2World& world, TextureManager& textures, const FontManager& fonts, SoundPlayer& soundPlayer):
	Weapon(world, soundPlayer, textures.getResourceReference(Textures_ID::Grenade), textures.getResourceReference(Textures_ID::GrenadeThumbnail), textures.getResourceReference(Textures_ID::GrenadeBullet), textures),
	fonts(fonts)
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
	setSparkColor(sf::Color::Cyan);
	setMaxDmg(110.f);
	setRange(450.f);
}

void Grenade::shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force)
{
	auto bullet = std::make_unique<Delayed_Bullet>(physicalWorld, soundPlayer, fonts, position, bulletTexture, textures, attackDmg, range, sf::seconds(8));
	bullet->setSparkColor(bulletSparksColor);
	bullet->applyForce(force);
	rootNode->pinNode(std::move(bullet));
}

bool Grenade::isActivation() const noexcept
{
	return false;
}

bool Grenade::isRoundEnding() const noexcept
{
	return true;
}
