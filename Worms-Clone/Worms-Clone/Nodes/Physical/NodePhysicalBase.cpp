#include "NodePhysicalBase.h"

#include "CollideTypes.h"

const float NodePhysicalBase::B2_SCALAR = 30.f;

NodePhysicalBase::NodePhysicalBase(b2World& world):
	World(&world)
{

}


void NodePhysicalBase::update(sf::Time deltaTime)
{
	NodeScene::update(deltaTime);
	updatePhysics();
}

float NodePhysicalBase::angleToRadians(const float& angle)
{
	return angle * (b2_pi / 180.0);
}

float NodePhysicalBase::radiansToAngle(const float& radians)
{
	return radians * 180 / b2_pi;
}
