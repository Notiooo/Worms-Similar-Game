#include "pch.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Cannon.h"

#include <SFML/Graphics/RenderTarget.hpp>


Cannon::Cannon(b2World& world, TextureManager& textures, SoundPlayer& soundPlayer):
	Weapon(world, soundPlayer, textures.getResourceReference(Textures_ID::Cannon), textures.getResourceReference(Textures_ID::CannonThumbnail), textures.getResourceReference(Textures_ID::CannonBullet), textures)
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
	setSparkColor(sf::Color::Black);
	setMaxDmg(300.f);
	setRange(80.f);
	setSound(Sound_ID::DefaultWeaponShoot);
}

bool Cannon::isActivation() const noexcept
{
	return false;
}

bool Cannon::isRoundEnding() const noexcept
{
	return true;
}
