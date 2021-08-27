#ifndef NODEDESTRUCTIBLERECTANGLE_H
#define NODEDESTRUCTIBLERECTANGLE_H

#include <clipper/clipper.hpp>
#include <poly2tri/common/shapes.h>
#include <SFML/Graphics/ConvexShape.hpp>

#include "Nodes/NodeScene.h"
#include "Nodes/Physical/NodePhysicalBody.h"


/**
 * \brief Deleter for the physical shape.
 */
struct PhysicalShapeDeleter
{
	void operator()(b2Vec2* ptr) const
	{
		delete[] ptr;
	}
};

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
	NodeDestructibleRectangle(b2World& world, const sf::Texture& texture, sf::Vector2f position, const sf::Vector2f& size);
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

	const sf::Texture& texture;
	
	std::vector<b2Fixture* > fixtures; //!< Attaches a shape to a body
	sf::VertexArray triangles; //!< Triangles (consisting of vertexes) building the current figure.
	std::vector<std::vector<p2t::Point*>> polyline; //!< Many sets of points, where each set builds the contour of the figure one by one
	std::vector<std::unique_ptr<b2Vec2[], PhysicalShapeDeleter>> physicalShape; //!< Physical shape (contour) that builds the shell of the object

	bool recalculateBody = false; //!< Flag checking if body should be recalculated after hit
	sf::Vector2f originTransform; //!< Displacement of an object so that the object is positioned relative to the centre
};

#endif