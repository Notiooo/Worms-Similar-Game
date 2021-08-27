#include "pch.h"
#include "World/WorldListener.h"

#include "Nodes/Physical/CollideTypes.h"
#include "Nodes/Physical/NodeDestructibleRectangle.h"
#include "Nodes/Physical/Specified/Worm/Worm.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Bullet.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Hitbox.h"

void WorldListener::BeginContact(b2Contact* contact)
{
	// Takes information about the occured collision
	const auto& userData1 = contact->GetFixtureA()->GetUserData();
	const auto& userData2 = contact->GetFixtureB()->GetUserData();

	// All collision informations should be stored as Collision*
	// So I cast it to this object that it supposed to be.
	auto* node1 = reinterpret_cast<Collision*>(userData1.pointer);
	auto* node2 = reinterpret_cast<Collision*>(userData2.pointer);

	for (const auto& node : {node1, node2})
	{
		if (node == nullptr)
			continue;

		switch (node->type)
		{
		case CollideTypes::WormFoot:
			{
				// I'm looking for collisions of my worm's feet
				// If it is my worm then it should dynamic_cast
				if (auto* worm = dynamic_cast<Worm*>(node->object))
					++(worm->footCollisions);
				break;
			}

		case CollideTypes::Water:
			{
				if (node1 == nullptr || node2 == nullptr)
					break;

				auto* toDelete = (node1->type != CollideTypes::Water) ? node1 : node2;
				toDelete->object->setDestroyed();
				break;
			}

		case CollideTypes::Bullet:
			{
				if (auto* bullet = dynamic_cast<Bullet*>(node->object))
					bullet->collision();
				break;
			}

		case CollideTypes::Hitbox:
			{
				if (node1 == nullptr || node2 == nullptr)
					break;

				// Hitbox with worm collision
				auto* wormCollision = (node1->type == CollideTypes::WormBody) ? node1 : node2;
				auto* hitbox = dynamic_cast<Hitbox*>(node->object);
				if (auto* worm = dynamic_cast<Worm*>(wormCollision->object))
				{
					worm->activateState(State_ID::WormHitState);

					// First I calculate distance from the explosion
					const auto distanceFromExplosion = (hitbox->getAbsolutePosition() - worm->getAbsolutePosition());

					// Then I had to take under consideration that object closer (with smaller distance) should be pushed further
					const auto rangeOfHitbox = hitbox->areaOfRange / 2.f / NodePhysicalBody::B2_SCALAR;

					sf::Vector2f force_vector;
					force_vector.x = (distanceFromExplosion.x)
						                 ? (rangeOfHitbox - distanceFromExplosion.x)
						                 : -(rangeOfHitbox + distanceFromExplosion.x);
					force_vector.y = (distanceFromExplosion.y)
						                 ? (rangeOfHitbox - distanceFromExplosion.y)
						                 : -(rangeOfHitbox + distanceFromExplosion.y);

					const auto distance = std::sqrt(
						distanceFromExplosion.x * distanceFromExplosion.x + distanceFromExplosion.y *
						distanceFromExplosion.y);

					// The force has to be negative to push AWAY from the explosion
					worm->applyForce(force_vector * 300.f);
					worm->setDamage(hitbox->maxDmg * (hitbox->areaOfRange - distance) / hitbox->areaOfRange);
					break;
				}

				// Hitbox with destructible node collision
				auto* groundCollision = (node1->type == CollideTypes::DestructibleGround) ? node1 : node2;
				if (auto* destructibleNode = dynamic_cast<NodeDestructibleRectangle*>(groundCollision->object))
				{
					std::vector<ClipperLib::IntPoint> figureCollision;
					for (double angle = 0; angle <= 2 * 3.14159; angle += 0.1)
						figureCollision.emplace_back(ClipperLib::IntPoint(
							hitbox->getAbsolutePosition().x + hitbox->areaOfRange / 2.f * std::cos(angle),
							hitbox->getAbsolutePosition().y + hitbox->areaOfRange / 2.f * std::sin(angle)));

					destructibleNode->addHole(figureCollision);
					break;
				}
				break;
			}
		}
	}
}

void WorldListener::EndContact(b2Contact* contact)
{
	// Takes information about the occured collision
	const auto& userData1 = contact->GetFixtureA()->GetUserData();
	const auto& userData2 = contact->GetFixtureB()->GetUserData();

	// All collision informations should be stored as NodeScene
	// So I cast it to this object that it supposed to be.
	auto* node1 = reinterpret_cast<Collision*>(userData1.pointer);
	auto* node2 = reinterpret_cast<Collision*>(userData2.pointer);

	for (const auto& node : {node1, node2})
	{
		if (node == nullptr)
			continue;

		switch (node->type)
		{
		case CollideTypes::WormFoot:
			{
				// I'm looking for collisions of my worm's feet
				// If it is my worm then it should dynamic_cast
				if (auto* worm = dynamic_cast<Worm*>(node->object))
					--(worm->footCollisions);
				break;
			}
		}
	}
}
