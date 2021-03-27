#include "NodePhysicalBody.h""

#include "CollideTypes.h"
#include "SFML/System/Vector2.hpp"

NodePhysicalBody::NodePhysicalBody(b2World& world, Physical_Types physical_type, sf::Vector2f position ):
	NodePhysicalBase(world),
	physicalType(physical_type)
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

NodePhysicalBody::~NodePhysicalBody()
{
	for (b2Fixture* fix = Body->GetFixtureList(); fix; fix = fix->GetNext())
	{
		delete reinterpret_cast<Collision*>(fix->GetUserData().pointer);
	}
	World->DestroyBody(Body);
}

void NodePhysicalBody::updatePhysics()
{
	this->setPosition(B2_SCALAR * Body->GetPosition().x, B2_SCALAR * Body->GetPosition().y);
	//this->setRotation(radiansToAngle(Body->GetAngle()));
	this->NodeScene::setRotation(radiansToAngle(Body->GetAngle()));
}


void NodePhysicalBody::applyForce(sf::Vector2f vector)
{
	Body->ApplyForceToCenter(sfVectorToB2Vec(vector), true);
}

void NodePhysicalBody::setRotation(float angle)
{
	// Do not change position, just change the angle
	Body->SetTransform(Body->GetPosition(), angleToRadians(angle));
}
