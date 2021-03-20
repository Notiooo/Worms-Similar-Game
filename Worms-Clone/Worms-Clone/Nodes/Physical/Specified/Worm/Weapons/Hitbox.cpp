#include "Hitbox.h"
#include "../../../CollideTypes.h"


Hitbox::Hitbox(b2World& world, sf::Vector2f position, float area_of_range, float force):
	NodePhysical(world, NodePhysical::Physical_Types::Static_Type, position),
	area_of_range(area_of_range),
	force(force)
{

	b2CircleShape circleShape;
	circleShape.m_radius = area_of_range;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.isSensor = true;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::Hitbox, *this));
	Body->CreateFixture(&fixtureDef);
}

bool Hitbox::isDestroyed()
{
	return true;
}
