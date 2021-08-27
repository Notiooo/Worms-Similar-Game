#ifndef NODEPHYSICALBODY_H
#define NODEPHYSICALBODY_H

#include "Nodes/Physical/NodePhysicalBase.h"

/**
 * \brief A node containing a physical body inside a simulation of the physical world.
 *
 * It can be one of three physical types. It synchronises with the
 * physical simulation on an ongoing basis.
 */
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
	~NodePhysicalBody() override;

	/**
	 * \brief Synchronises a graphical object with a simulation inside the physical world.
	 */
	void updatePhysics() override;

	/**
	 * \brief Applies force under the indicated vector.
	 * The force is directed towards the centre of the object.
	 * \param vector Vector with force to be applied
	 */
	void applyForce(sf::Vector2f vector);


	/**
	 * \brief Rotates the physical objects
	 * \param angle The angle at which the object should be tilted.
	 */
	void setRotation(float angle);

protected:
	const Physical_Types physicalType; //!< Defines the physical type of this object
	b2Body* Body; //!< The physical body of an object inside a simulation of the physical world

};

#endif