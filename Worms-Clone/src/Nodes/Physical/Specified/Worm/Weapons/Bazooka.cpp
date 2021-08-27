#include "pch.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Bazooka.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Nodes/Physical/CollideTypes.h"


Bazooka::Bazooka(b2World& world, TextureManager& textures, SoundPlayer& soundPlayer):
	Weapon(world, soundPlayer, textures.getResourceReference(Textures_ID::Bazooka), textures.getResourceReference(Textures_ID::BazookaThumbnail), textures.getResourceReference(Textures_ID::BazookaBullet), textures)
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
	setSparkColor(sf::Color::Blue);
	setMaxDmg(80.f);
	setRange(220.f);
	setSound(Sound_ID::DefaultWeaponShoot);
}

bool Bazooka::isActivation() const noexcept
{
	return false;
}

bool Bazooka::isRoundEnding() const noexcept
{
	return true;
}

