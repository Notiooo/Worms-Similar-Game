#include "WorldListener.h"

#include "Nodes/Physical/CollideTypes.h"
#include "Nodes/Physical/Specified/Worm/Worm.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Bullet.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Hitbox.h"

void WorldListener::BeginContact(b2Contact* contact)
{
	// Takes information about the occured collision
	b2FixtureUserData userData1 = contact->GetFixtureA()->GetUserData();
	b2FixtureUserData userData2 = contact->GetFixtureB()->GetUserData();

	// All collision informations should be stored as Collision*
	// So I cast it to this object that it supposed to be.
	Collision* node1 = reinterpret_cast<Collision*>(userData1.pointer);
	Collision* node2 = reinterpret_cast<Collision*>(userData2.pointer);

	for (auto& node : { node1, node2 })
	{
		if (node == nullptr)
			continue;

		switch (node->type)
		{
		case CollideTypes::WormFoot:
		{
			// I'm looking for collisions of my worm's feet
			// If it is my worm then it should dynamic_cast
			if (Worm* worm = dynamic_cast<Worm*>(node->object))
				++(worm->footCollisions);
			break;
		}

		case CollideTypes::Bullet:
		{
			if (Bullet* bullet = dynamic_cast<Bullet*>(node->object))
				bullet->setDestroyed();
			break;
		}

		case CollideTypes::Hitbox:
		{
			if (node1 == nullptr || node2 == nullptr)
				break;

			Collision* worm_collision = (node1->type == CollideTypes::WormBody) ? node1 : node2;
			Hitbox* hitbox = dynamic_cast<Hitbox*>(node->object);
			if (Worm* worm = dynamic_cast<Worm*>(worm_collision->object))
			{
				worm->activateState(State_ID::WormHitState);

				// First I calculate distance from the explosion
				const sf::Vector2f distance_from_explosion = (hitbox->getAbsolutePosition() - worm->getAbsolutePosition());

				// Then I had to take under consideration that object closer (with smaller distance) should be pushed further
				const float range_of_hitbox = hitbox->areaOfRange / 2.f / NodePhysicalBody::B2_SCALAR;

				sf::Vector2f force_vector;
				force_vector.x = (distance_from_explosion.x) ? (range_of_hitbox - distance_from_explosion.x) : -(range_of_hitbox + distance_from_explosion.x);
				force_vector.y = (distance_from_explosion.y) ? (range_of_hitbox - distance_from_explosion.y) : -(range_of_hitbox + distance_from_explosion.y);

				float distance = std::sqrt(distance_from_explosion.x * distance_from_explosion.x + distance_from_explosion.y * distance_from_explosion.y);
				// The force has to be negative to push AWAY from the explosion
				worm->applyForce(force_vector * 300.f);
				worm->setDamage(hitbox->maxDmg * (hitbox->areaOfRange - distance) / hitbox->areaOfRange);
			}

			break;
		}
		}
	}
}

void WorldListener::EndContact(b2Contact* contact)
{
	// Takes information about the occured collision
	b2FixtureUserData userData1 = contact->GetFixtureA()->GetUserData();
	b2FixtureUserData userData2 = contact->GetFixtureB()->GetUserData();

	// All collision informations should be stored as NodeScene
	// So I cast it to this object that it supposed to be.
	Collision* node1 = reinterpret_cast<Collision*>(userData1.pointer);
	Collision* node2 = reinterpret_cast<Collision*>(userData2.pointer);

	for (auto& node : { node1, node2 })
	{
		if (node == nullptr)
			continue;

		switch (node->type)
		{
		case CollideTypes::WormFoot:
		{
			// I'm looking for collisions of my worm's feet
			// If it is my worm then it should dynamic_cast
			if (Worm* worm = dynamic_cast<Worm*>(node->object))
				--(worm->footCollisions);
			break;
		}
		}
	}
}