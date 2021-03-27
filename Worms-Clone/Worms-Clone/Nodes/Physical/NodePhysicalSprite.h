#ifndef NODEPHYSICALSPRITE_H
#define NODEPHYSICALSPRITE_H

#include "NodePhysicalBody.h"
#include "SFML/Graphics/Sprite.hpp"

class NodePhysicalSprite : public NodePhysicalBody
{
public:
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Vector2f position, sf::Texture& texture);
	
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;

protected:
	sf::Sprite sprite;

};
#endif