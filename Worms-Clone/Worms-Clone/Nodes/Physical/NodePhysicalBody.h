#ifndef NODEPHYSICALBODY_H
#define NODEPHYSICALBODY_H

#include "NodePhysicalBase.h"
#include "box2d/box2d.h"

class NodePhysicalBody : public NodePhysicalBase
{
public:
	enum class Physical_Types
	{
		Kinematic_Type, //!< can move but it will not be affected by other bodies
		Static_Type, //!< never moves
		Dynamic_Type, //!< body can interact with other bodies
	};

	NodePhysicalBody(b2World& world, Physical_Types physical_type, sf::Vector2f position);
	~NodePhysicalBody();

	void updatePhysics() override;

	void applyForce(sf::Vector2f vector);


	/**
	 * \brief Rotates the physical objects
	 * \param angle The angle at which the object should be tilted.
	 */
	void setRotation(float angle);

protected:
	const Physical_Types physicalType;
	b2Body* Body;

};

#endif