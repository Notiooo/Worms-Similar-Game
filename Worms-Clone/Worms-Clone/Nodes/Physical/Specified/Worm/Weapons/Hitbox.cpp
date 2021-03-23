#include "Hitbox.h"
#include "../../../CollideTypes.h"


Hitbox::Hitbox(b2World& world, sf::Vector2f position, float area_of_range, float max_dmg):
	NodePhysical(world, NodePhysical::Physical_Types::Kinematic_Type, position),
	areaOfRange(area_of_range),
	maxDmg(max_dmg)
{

	b2CircleShape circleShape;
	circleShape.m_radius = area_of_range / 2.f / B2_SCALAR;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.isSensor = true;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::Hitbox, *this));
	fixtureDef.filter.groupIndex = -1;
	Body->CreateFixture(&fixtureDef);
	updatePhysics();
}

void Hitbox::updateThis(sf::Time deltaTime)
{
	// Destroy in next iteration
	updatePhysics();
	setDestroyed();
}
