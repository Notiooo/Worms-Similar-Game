#ifndef HITBOX_H
#define HITBOX_H

#pragma warning(push, 0)
#include "box2d/box2d.h"
#pragma warning(pop)

#include "../../../NodePhysicalBody.h"

class Hitbox : public NodePhysicalBody
{
public:
	friend class WorldListener;
	Hitbox(b2World& world, sf::Vector2f position, float area_of_range,float max_dmg);

	void updateThis(sf::Time deltaTime) override;
	

private:
	// How big area it affects
	float areaOfRange;

	// How much damage it takes
	float maxDmg;

};



#endif