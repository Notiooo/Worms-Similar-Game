#ifndef COLLIDETYPES_H
#define COLLIDETYPES_H

#include "../../Nodes/NodeScene.h"

// I changed my mind, it wont be deprecated

enum class CollideTypes
{
	// Worm related
	WormFoot,
	WormBody,

	// Weapons
	Bullet,
	Hitbox,
};

struct Collision
{
	CollideTypes type;
	NodeScene* object;

	Collision(CollideTypes type, NodeScene& object) : type(type), object(&object) {}
	explicit operator Collision* () { return this; };
};

#endif