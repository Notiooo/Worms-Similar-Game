#ifndef HITBOX_H
#define HITBOX_H

#pragma warning(push, 0)
#include "box2d/box2d.h"
#pragma warning(pop)

#include "../../../NodePhysicalBody.h"

/**
 * \brief An object that reports a collision within its boundary. Used to find worm hit by an explosion.
 * Removes itself in the next iteration of the game.
 */
class Hitbox : public NodePhysicalBody
{
public:
	friend class WorldListener; //!< Class that handles collisions inside the game
	
	Hitbox(b2World& world, sf::Vector2f position, float area_of_range,float max_dmg);

	void updateThis(sf::Time deltaTime) override;
	

private:
	float areaOfRange; //!< How large an area will the hitbox cover. Size measured in pixels as radius.
	float maxDmg; //!< How much damage it should do to the worm

};



#endif