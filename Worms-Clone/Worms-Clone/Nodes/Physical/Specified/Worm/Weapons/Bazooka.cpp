#include "Bazooka.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "../../../CollideTypes.h"
#include <memory>


Bazooka::Bazooka(b2World& world, TextureManager& textures):
	Weapon(world, textures.getResourceReference(Textures_ID::Bazooka), textures.getResourceReference(Textures_ID::Bazooka_Bullet))
{
	weaponSprite.setPosition(getPosition().x, getPosition().y + 40);
}

void Bazooka::shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(physical_world, position, bulletTexture, attack_dmg, range);
	bullet->applyForce(force);
	rootNode->pinNode(std::move(bullet));
}

void Bazooka::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(weaponSprite, states);
}

void Bazooka::updateThis(sf::Time deltaTime)
{
}

