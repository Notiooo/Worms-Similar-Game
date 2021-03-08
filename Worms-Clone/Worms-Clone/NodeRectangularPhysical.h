#ifndef NODERECTANGULARPHYSICAL_H
#define NODERECTANGULARPHYSICAL_H

#include "Nodes/NodeScene.h"
#include "SFML/Graphics/RectangleShape.hpp"

#include "box2d/box2d.h"


class NodeRectangularPhysical : public NodeScene
{
public:
	enum class Physical_Types
	{
		Kinematic_Type, // can move but it will not be affected by other bodies
		Static_Type, // never moves
		Dynamic_Type, // body can interact with other bodies
	};

	NodeRectangularPhysical(b2World& world, sf::Vector2f size, sf::Vector2f position, sf::Color color, Physical_Types physical_type = Physical_Types::Kinematic_Type);

	virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateThis(sf::Time deltaTime) override;

private:
	sf::RectangleShape rectangle;
	Physical_Types physical_type;
	static const float B2_SCALAR;

	b2Body* Body;
};


#endif