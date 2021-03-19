#include "NodePhysical.h""
#include "SFML/System/Vector2.hpp"

const float NodePhysical::B2_SCALAR = 30.f;
std::queue<b2Body*> NodePhysical::b2_removal_queue;

NodePhysical::NodePhysical(b2World& world, Physical_Types physical_type, sf::Vector2f position ):
	physical_type(physical_type),
	World(&world)
{
	// Defines properties of the body
	b2BodyDef BodyDef;
	BodyDef.position = sfVector_to_b2Vec(position);
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

NodePhysical::~NodePhysical()
{
	b2_removal_queue.push(Body);
}

float NodePhysical::angleToRadians(const float& angle)
{
	return angle * (b2_pi / 180.0);
}

float NodePhysical::radiansToAngle(const float& radians)
{
	return radians * 180 / b2_pi;
}

void NodePhysical::applyForce(sf::Vector2f vector)
{
	Body->ApplyForceToCenter(sfVector_to_b2Vec(vector), true);
}

