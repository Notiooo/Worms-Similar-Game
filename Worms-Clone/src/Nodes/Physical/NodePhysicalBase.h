#ifndef NODEPHYSICALBASE_H
#define NODEPHYSICALBASE_H

#include "Nodes/NodeScene.h"
#pragma warning(push, 0)
#include <box2d/box2d.h>
#pragma warning(pop)

/**
 * \brief An abstract class of physical object.
 *
 * Contains helper functions to convert between angles, radians and
 * units of SFML and BOX2D libraries
 */
class NodePhysicalBase : public NodeScene
{
public:
	/**
	 * \brief It is used to scale meters to pixels
	 */
	static const float B2_SCALAR;

	NodePhysicalBase(b2World& world);

	void update(sf::Time deltaTime) override final;
	virtual void updatePhysics() = 0;
	
	/**
	 * \brief Converts from pixels to meters
	 * \tparam Vector2 Vector from SFML Library
	 * \param vec Vector from SFML Library
	 * \return Vector from Box2D library
	 */
	template <typename Vector2>
	static b2Vec2 sfVectorToB2Vec(const Vector2& vec);
	
	/**
	 * \brief Converts from meters to pixels
	 * \tparam Vector2 Vector from SFML Library
	 * \param vec Vector from SFML Library
	 * \return Vector from Box2D library
	 */
	template <typename Vector2>
	static Vector2 b2VecToSfVector(const b2Vec2& vec);

	/**
	 * \brief Converts from angle to radians
	 * \param angle angle given in degrees
	 * \return Angle given in radians
	 *
	 * Useful for conversion between Box2D and the SFML
	 */
	static float angleToRadians(const float& angle);

	/**
	 * \brief Converts from radians to angles
	 * \param radians angle given in radians
	 * \return Angle given in degrees
	 *
	 * Useful for conversion between Box2D and the SFML
	 */
	static float radiansToAngle(const float& radians);

protected:
	b2World* World; //!< Simulation of the physical world
};


template<typename Vector2>
b2Vec2 NodePhysicalBase::sfVectorToB2Vec(const Vector2& vec)
{
	return b2Vec2(vec.x / B2_SCALAR, vec.y / B2_SCALAR);
}

template<typename Vector2>
Vector2 NodePhysicalBase::b2VecToSfVector(const b2Vec2& vec)
{
	return Vector2(vec.x * B2_SCALAR, vec.y * B2_SCALAR);
}

#endif