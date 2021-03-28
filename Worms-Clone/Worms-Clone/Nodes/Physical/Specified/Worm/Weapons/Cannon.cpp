#include "Cannon.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include <memory>


Cannon::Cannon(b2World& world, TextureManager& textures):
	Weapon(world, textures.getResourceReference(Textures_ID::Cannon), textures.getResourceReference(Textures_ID::CannonThumbnail), textures.getResourceReference(Textures_ID::CannonBullet))
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
	setSparkColor(sf::Color::Black);
	setMaxDmg(300.f);
	setRange(80.f);
}

bool Cannon::isActivation()
{
	return false;
}

bool Cannon::isRoundEnding()
{
	return true;
}
