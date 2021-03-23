#ifndef NODEPHYSICAL_H
#define NODEPHYSICAL_H

#include "../NodeScene.h"
#include "box2d/box2d.h"
#include "CollideTypes.h"
#include <queue>

class NodePhysical : public NodeScene
{
public:
	// It is used to scale meters to pixels
	static const float B2_SCALAR;

	enum class Physical_Types
	{
		Kinematic_Type, // can move but it will not be affected by other bodies
		Static_Type, // never moves
		Dynamic_Type, // body can interact with other bodies
	};

	NodePhysical(b2World& world, Physical_Types physical_type, sf::Vector2f position);
	~NodePhysical();

	void updateThis(sf::Time deltaTime) override;
	void updatePhysics();
	
	// Converts from pixels to meters
	template <typename Vector2>
	static b2Vec2 sfVectorToB2Vec(const Vector2& vec);

	// Converts from meters to pixels
	template <typename Vector2>
	static Vector2 b2VecToSfVector(const b2Vec2& vec);

	// Converts from angle to radians -- useful for conversion
	// between Box2D and the SFML
	static float angleToRadians(const float& angle);

	// Converts from radians to angles -- useful for conversion
	// between Box2D and the SFML
	static float radiansToAngle(const float& radians);

	void applyForce(sf::Vector2f vector);

protected:
	const Physical_Types physicalType;
	b2Body* Body;
	b2World* World;

};


template<typename Vector2>
inline b2Vec2 NodePhysical::sfVectorToB2Vec(const Vector2& vec)
{
	return b2Vec2(vec.x / B2_SCALAR, vec.y / B2_SCALAR);
}

template<typename Vector2>
inline Vector2 NodePhysical::b2VecToSfVector(const b2Vec2& vec)
{
	return Vector2(vec.x * B2_SCALAR, vec.y * B2_SCALAR);
}

#endif