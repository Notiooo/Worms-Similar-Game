#ifndef NODEDESTRUCTIBLERECTANGLE_H
#define NODEDESTRUCTIBLERECTANGLE_H

#include <clipper.hpp>
#include <common/shapes.h>
#include <SFML/Graphics/ConvexShape.hpp>

#include "../NodeScene.h"
#include "../Physical/NodePhysicalBody.h"

/**
 * \brief A node that has the properties of a static physical object -- it physically
 * interacts with other physical objects and can be damaged inside the game.
 *
 * This object has a function that calculates the difference with another figure
 * and removes the overlapping part of the object.
 */
class NodeDestructibleRectangle : public NodePhysicalBody
{
public:
	NodeDestructibleRectangle(b2World& world, sf::Vector2f position, const sf::Vector2f& size);
	~NodeDestructibleRectangle() override;
	
	/**
	 * \brief Calculates the overlapping part of both figures and performs the
	 * difference operation (removes the place where the figures overlap).
	 * \param figure Points that build a figure which at the intersection removes the current part of the node.
	 */
	void addHole(std::vector<ClipperLib::IntPoint>& figure);

	
	/**
	 * \brief It updates the current state of the object, synchronises it
	 * with its physical form and checks if it needs recalculation after impact.
	 * \param deltaTime Time elapsed since the previous frame
	 */
	void updateThis(sf::Time deltaTime) override;

private:
	/**
	 * \brief Draws this object (all its triangles).
	 */
	void drawThis(sf::RenderTarget&, sf::RenderStates) const override;


	/**
	 * \brief Creates the physical body of an object
	 */
	void createBody();
	
	b2Fixture* fixture; //!< Attaches a shape to a body
	std::vector<sf::ConvexShape> drawableTriangles; //!< Graphic triangles building up the whole visual part of the object
	std::vector<p2t::Point*> polyline; //!< Points that build the contour of the figure one by one
	std::vector<p2t::Triangle*> triangles; //!< Triangles (consisting of points) building the current figure.
	b2Vec2* physicalShape = nullptr;

	bool recalculateBody = false; //!< Flag checking if body should be recalculated after hit
	sf::Vector2f originTransform; //!< Displacement of an object so that the object is positioned relative to the centre
};

#endif