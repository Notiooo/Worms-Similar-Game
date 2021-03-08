#ifndef NODEPHYSICAL_H
#define NODEPHYSICAL_H

#include "../NodeScene.h"
#include "box2d/box2d.h"

class NodePhysical : public NodeScene
{
public:
	static const float B2_SCALAR;
	enum class Physical_Types
	{
		Kinematic_Type, // can move but it will not be affected by other bodies
		Static_Type, // never moves
		Dynamic_Type, // body can interact with other bodies
	};

	NodePhysical(b2World& world, Physical_Types physical_type, sf::Vector2f position);

protected:
	const Physical_Types physical_type;
	b2Body* Body;

};


#endif