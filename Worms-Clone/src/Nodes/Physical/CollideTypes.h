#ifndef COLLIDETYPES_H
#define COLLIDETYPES_H

#include "Nodes/NodeScene.h"

/**
 * \brief Specifies which object collides
 */
enum class CollideTypes
{
	// World
	Water,
	DestructibleGround,
	
	// Worm related
	WormFoot,
	WormBody,

	// Weapons
	Bullet,
	Hitbox,
};

/**
 * \brief A collision object that contains information about what object
 * is colliding, and stores a pointer to that object.
 */
struct Collision
{
	CollideTypes type;
	NodeScene* object;

	Collision(CollideTypes type, NodeScene& object) : type(type), object(&object) {}
	explicit operator Collision* () { return this; };
};

#endif