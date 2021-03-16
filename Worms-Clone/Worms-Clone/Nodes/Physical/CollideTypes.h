#ifndef COLLIDETYPES_H
#define COLLIDETYPES_H
#include <iostream>

// Not used yet, might be deprecated
// in next commits

enum class CollideTypes
{
	WormFoot,
	WormBody,
};

struct Collision
{
	CollideTypes type;
	NodeScene* object;

	Collision(CollideTypes type, NodeScene& object) : type(type), object(&object) {}
	explicit operator Collision* () { return this; };
	
	~Collision() { std::cout << "delete" << std::endl; }
};

#endif