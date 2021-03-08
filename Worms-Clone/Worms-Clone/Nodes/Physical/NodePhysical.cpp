#include "NodePhysical.h""
#include "SFML/System/Vector2.hpp"

const float NodePhysical::B2_SCALAR = 30.f;

NodePhysical::NodePhysical(b2World& world, Physical_Types physical_type, sf::Vector2f position ):
	physical_type(physical_type)
{
	// Defines properties of the body
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(position.x / B2_SCALAR, position.y / B2_SCALAR);
	switch (physical_type)
	{
	case Physical_Types::Kinematic_Type:
		BodyDef.type = b2_kinematicBody;
		break;
	case Physical_Types::Static_Type:
		BodyDef.type = b2_staticBody;
		break;
	case Physical_Types::Dynamic_Type:
		BodyDef.type = b2_dynamicBody;
		break;
	}
	Body = world.CreateBody(&BodyDef);
}
