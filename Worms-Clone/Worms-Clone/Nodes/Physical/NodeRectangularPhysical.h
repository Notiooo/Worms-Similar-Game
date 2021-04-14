#ifndef NODERECTANGULARPHYSICAL_H
#define NODERECTANGULARPHYSICAL_H

#include "NodePhysicalBody.h"
#include "SFML/Graphics/RectangleShape.hpp"


/**
 * \brief A physical and graphical object representing a rectangle within a game.
 */
class NodeRectangularPhysical : public NodePhysicalBody
{
public:
	NodeRectangularPhysical(b2World& world, sf::Vector2f size, sf::Vector2f position, sf::Color color, Physical_Types physical_type = Physical_Types::Kinematic_Type);

	/**
	 * \brief Draws only this node to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RectangleShape rectangle; //!< Graphical representation of a rectangle
};


#endif