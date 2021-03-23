#ifndef WORLDLISTENER_H
#define WORLDLISTENER_H

#include <box2d/b2_world_callbacks.h>

class WorldListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};


#endif