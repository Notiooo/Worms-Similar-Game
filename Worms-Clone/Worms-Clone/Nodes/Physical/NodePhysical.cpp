#include "NodePhysical.h""
#include "SFML/System/Vector2.hpp"

const float NodePhysical::B2_SCALAR = 30.f;

NodePhysical::NodePhysical(b2World& world, Physical_Types physical_type, sf::Vector2f position ):
	physicalType(physical_type),
	World(&world)
{
	// Defines properties of the body
	b2BodyDef BodyDef;
	BodyDef.position = sfVectorToB2Vec(position);
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
	for (b2Fixture* fix = Body->GetFixtureList(); fix; fix = fix->GetNext())
	{
		delete reinterpret_cast<Collision*>(fix->GetUserData().pointer);
	}
	World->DestroyBody(Body);
}

void NodePhysical::updateThis(sf::Time deltaTime)
{
	updatePhysics();
}

void NodePhysical::updatePhysics()
{
	this->setPosition(B2_SCALAR * Body->GetPosition().x, B2_SCALAR * Body->GetPosition().y);
	this->setRotation(Body->GetAngle() * 180 / b2_pi);
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
	Body->ApplyForceToCenter(sfVectorToB2Vec(vector), true);
}

