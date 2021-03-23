#ifndef NODERECTANGULARPHYSICAL_H
#define NODERECTANGULARPHYSICAL_H

#include "NodePhysical.h"
#include "SFML/Graphics/RectangleShape.hpp"


class NodeRectangularPhysical : public NodePhysical
{
public:
	NodeRectangularPhysical(b2World& world, sf::Vector2f size, sf::Vector2f position, sf::Color color, Physical_Types physical_type = Physical_Types::Kinematic_Type);

	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;

	void setRotation(float angle);

private:
	sf::RectangleShape rectangle;
};


#endif