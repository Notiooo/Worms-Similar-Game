#ifndef WORLDLISTENER_H
#define WORLDLISTENER_H

#include <box2d/b2_world_callbacks.h>

/**
 * \brief Checks for and handles collisions within the game
 */
class WorldListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};


#endif