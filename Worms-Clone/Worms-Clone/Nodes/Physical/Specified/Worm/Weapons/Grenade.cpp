#include "Grenade.h"


#include "Delayed_Bullet.h"
#include "../../../../GameplayManager.h"
#include "SFML/Graphics/RenderTarget.hpp"


Grenade::Grenade(b2World& world, TextureManager& textures, const FontManager& fonts):
	Weapon(world, textures.getResourceReference(Textures_ID::Grenade), textures.getResourceReference(Textures_ID::GrenadeThumbnail), textures.getResourceReference(Textures_ID::GrenadeBullet), textures),
	fonts(fonts)
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
	setSparkColor(sf::Color::Cyan);
	setMaxDmg(110.f);
	setRange(450.f);
}

void Grenade::shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force)
{
	std::unique_ptr<Delayed_Bullet> bullet = std::make_unique<Delayed_Bullet>(physicalWorld, fonts, position, bulletTexture, textures, attackDmg, range, sf::seconds(8));
	bullet->setSparkColor(bulletSparksColor);
	bullet->applyForce(force);
	rootNode->pinNode(std::move(bullet));
}

bool Grenade::isActivation()
{
	return false;
}

bool Grenade::isRoundEnding()
{
	return true;
}
