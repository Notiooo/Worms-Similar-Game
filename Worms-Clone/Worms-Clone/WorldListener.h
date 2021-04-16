#ifndef WORLDLISTENER_H
#define WORLDLISTENER_H

#include <box2d/b2_world_callbacks.h>

/**
 * \brief Checks for and handles collisions within the game
 */
class WorldListener final : public b2ContactListener
{
	/**
	 * \brief A function that executes when a collision starts.
	 * \param contact Information on the collision that took place
	 */
	void BeginContact(b2Contact* contact) override;
	/**
	 * \brief A function that executes when a collision end
	 * \param contact Information on the collision that took place
	 */
	void EndContact(b2Contact* contact) override;
};


#endif