#ifndef NODEPHYSICALSPRITE_H
#define NODEPHYSICALSPRITE_H

#include "NodePhysicalBody.h"
#include "SFML/Graphics/Sprite.hpp"

class NodePhysicalSprite : public NodePhysicalBody
{
public:
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture, sf::Vector2f position);
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture, sf::Vector2f position, const sf::IntRect& rect);
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture, sf::Vector2f position, const sf::Vector2f& size);
	
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;

	void setSize(const sf::IntRect&);

private:
	void createBody();
	void centerOrigin();

protected:
	sf::Sprite sprite;
	b2Fixture* fixture;

};
#endif