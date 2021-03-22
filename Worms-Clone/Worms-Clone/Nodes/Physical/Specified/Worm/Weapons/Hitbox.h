#ifndef HITBOX_H
#define HITBOX_H

#include "box2d/box2d.h"
#include "../../../NodePhysical.h"

class Hitbox : public NodePhysical
{
public:
	friend class WorldListener;
	Hitbox(b2World& world, sf::Vector2f position, float area_of_range,float max_dmg);

	virtual void updateThis(sf::Time deltaTime) override;
	

private:
	// How big area it affects
	float area_of_range;

	// How much damage it takes
	float max_dmg;

};



#endif