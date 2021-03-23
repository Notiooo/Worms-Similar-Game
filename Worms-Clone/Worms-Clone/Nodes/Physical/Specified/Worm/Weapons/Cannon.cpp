#include "Cannon.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include <memory>


Cannon::Cannon(b2World& world, TextureManager& textures):
	Weapon(world, textures.getResourceReference(Textures_ID::Cannon), textures.getResourceReference(Textures_ID::Cannon_Thumbnail), textures.getResourceReference(Textures_ID::Cannon_Bullet))
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
}

void Cannon::shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(physicalWorld, position, bulletTexture, attackDmg, range);
	bullet->applyForce(force);
	rootNode->pinNode(std::move(bullet));
}

void Cannon::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(weaponSprite, states);
}

void Cannon::updateThis(sf::Time deltaTime)
{
}

