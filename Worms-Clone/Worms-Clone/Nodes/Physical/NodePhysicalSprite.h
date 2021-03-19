#ifndef NODEPHYSICALSPRITE_H
#define NODEPHYSICALSPRITE_H

#include "NodePhysical.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

class NodePhysicalSprite : public NodePhysical
{
public:
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Vector2f position, sf::Texture& texture);
	
	virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateThis(sf::Time deltaTime) override;
	void setRotation(float angle);

protected:
	sf::Sprite sprite;

};
#endif