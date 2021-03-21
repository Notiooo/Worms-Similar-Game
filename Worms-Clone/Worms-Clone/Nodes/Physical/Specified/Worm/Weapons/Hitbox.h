#ifndef HITBOX_H
#define HITBOX_H

#include "box2d/box2d.h"
#include "../../../NodePhysical.h"

class Hitbox : public NodePhysical
{
public:
	friend class WorldListener;
	Hitbox(b2World& world, sf::Vector2f position, float area_of_range, float force);

	//virtual void updateThis(sf::Time deltaTime) override;
	bool isDestroyed() override;
	void setDestroyed();

	virtual void updateThis(sf::Time deltaTime) override;
	

private:
	// How big area it affects
	float area_of_range;

	// How powerful it is? (dmg and how far it pushes worms)
	float force;
	bool destroyed = false;
};



#endif