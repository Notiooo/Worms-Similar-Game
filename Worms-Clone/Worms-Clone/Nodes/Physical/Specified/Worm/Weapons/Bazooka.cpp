#include "Bazooka.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "../../../CollideTypes.h"
#include <memory>


Bazooka::Bazooka(b2World& world, TextureManager& textures):
	Weapon(world, textures.getResourceReference(Textures_ID::Bazooka), textures.getResourceReference(Textures_ID::BazookaThumbnail), textures.getResourceReference(Textures_ID::BazookaBullet), textures)
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
	setSparkColor(sf::Color::Blue);
	setMaxDmg(80.f);
	setRange(220.f);
}

bool Bazooka::isActivation() const noexcept
{
	return false;
}

bool Bazooka::isRoundEnding() const noexcept
{
	return true;
}

