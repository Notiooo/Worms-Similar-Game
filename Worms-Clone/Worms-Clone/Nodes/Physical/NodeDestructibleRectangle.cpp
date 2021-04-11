#include "NodeDestructibleRectangle.h"


#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "clipper.hpp"
#include "poly2tri.h"
#include "../Physical/CollideTypes.h"
#include "../Physical/NodePhysicalSpark.h"


NodeDestructibleRectangle::NodeDestructibleRectangle(b2World& world, sf::Vector2f position, const sf::Vector2f& size):
	NodePhysicalBody(world, Physical_Types::Dynamic_Type, position)
{
	// The centre of the object is calculated
	originTransform = {size.x / 2.f, size.y / 2.f};


	// Each point of the rectangle is stored in a vector of points which builds the contour of the figure.
	// The points are moved in such a way that the object is constructed relative to its centre (the centre
	// of the object is located at the x and y positions on which the object was to appear)
	polyline.push_back(new p2t::Point(0 - originTransform.x, 0 - originTransform.y));
	polyline.push_back(new p2t::Point(size.x - originTransform.x, 0 - originTransform.y));
	polyline.push_back(new p2t::Point(size.x - originTransform.x, size.y - originTransform.y));
	polyline.push_back(new p2t::Point(0 - originTransform.x, size.y - originTransform.y));


	// A triangulation process that transforms the outline/shape of an object into triangles.
	p2t::CDT cdt(polyline);
	cdt.Triangulate();

	// The resulting triangles are written in the vector
	triangles = cdt.GetTriangles();

	// The vector with graphical triangles is equal to their number and each triangle has three points
	drawableTriangles.resize(triangles.size(), sf::ConvexShape(3));

	// Each graphical triangle is assigned the coordinates of the calculated triangles obtained from the triangulation process.
	for (size_t i = 0; i < triangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
			drawableTriangles[i].setPoint(j, sf::Vector2f{
				                              static_cast<float>(triangles[i]->GetPoint(j)->x),
				                              static_cast<float>(triangles[i]->GetPoint(j)->y)
			                              });
	}

	// Finally the physical body can be made
	createBody();
}

NodeDestructibleRectangle::~NodeDestructibleRectangle()
{
	// I remove points that build the outline of an object
	for (auto& point : polyline)
		delete point;

	// I remove the vector that builds the PHYSICAL version of
	// the object (they are calculated differently).
	delete[] physicalShape;
	physicalShape = nullptr;
}

void NodeDestructibleRectangle::updateThis(sf::Time deltaTime)
{
	// Synchronize the drawable Rectangle with the physical object
	updatePhysics();

	// If an object has been hit then its physical form needs to be matched to its new - destroyed form.
	if (recalculateBody)
	{
		// This means that the current contour must have collision information removed,
		// which is allocated dynamically.
		for (b2Fixture* fix = Body->GetFixtureList(); fix; fix = fix->GetNext())
			delete reinterpret_cast<Collision*>(fix->GetUserData().pointer);

		// Then the set of points that build the physical contour of the object must be removed
		delete[] physicalShape;
		physicalShape = nullptr;

		// You can now remove this outline/contour
		Body->DestroyFixture(fixture);

		// And create new body
		createBody();
		recalculateBody = false;
	}
}

void NodeDestructibleRectangle::addHole(std::vector<ClipperLib::IntPoint>& figure)
{
	// First I will have to transform both figures in such a way that Clipper
	// can subtract them from each other.
	ClipperLib::Paths shape(1), clip(1), solution(1);

	for (auto& point : polyline)
	{
		// The point building the contour must be transformed in such
		// a way that its absolute position and rotation match.
		auto transformedPoint = getTransform().transformPoint(static_cast<float>(point->x),
		                                                      static_cast<float>(point->y));
		shape[0].emplace_back(ClipperLib::IntPoint(transformedPoint.x, transformedPoint.y));
	}

	// The figure should already be processed, so there is no problem with it
	for (auto& point : figure)
		clip[0].emplace_back(point);

	// I then carry out the operations to cut the object
	ClipperLib::Clipper c;
	c.AddPath(shape[0], ClipperLib::ptSubject, true);
	c.AddPath(clip[0], ClipperLib::ptClip, true);
	c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

	// After differentiation I remove current values of the triangle
	for (int i = 0; i < polyline.size(); ++i)
		delete polyline[i];
	polyline.clear();


	// If there are no points it means it got completely destroyed
	// So it can be removed from the game world
	if (solution.empty())
	{
		setDestroyed();
		return;
	}

	// Prepares a vector to a size where it can accommodate all the points
	// that make up the contour, the number of which is contained in the
	// result of subtracting the Clipper
	polyline.resize(solution[0].size());

	// Then I process all the points calculated by Clipper
	for (size_t i = 0; i < solution[0].size(); ++i)
	{
		// I process the points obtained so that they are again independent of absolute position
		// and rotation -- that is, I calculate the relativistic position for that object.
		auto transformedPoint = getInverseTransform().transformPoint(static_cast<float>(solution[0][i].X),
		                                                             static_cast<float>(solution[0][i].Y));
		polyline[i] = new p2t::Point(transformedPoint.x, transformedPoint.y);
	}

	// I can convert the points obtained in the subtraction process
	// into triangles after processing.
	p2t::CDT cdt(polyline);
	cdt.Triangulate();

	// Updates the set of triangles building this object.
	triangles = cdt.GetTriangles();

	// Again, it prepares a set of drawable triangles whose size
	// corresponds to the number of resulting triangles consisting
	// of (no surprise) three points
	drawableTriangles.resize(triangles.size(), sf::ConvexShape(3));


	// Each visual triangle has assigned points equal to those
	// obtained after the triangulation process.
	for (size_t i = 0; i < triangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
			drawableTriangles[i].setPoint(j, sf::Vector2f{
				                              static_cast<float>(triangles[i]->GetPoint(j)->x),
				                              static_cast<float>(triangles[i]->GetPoint(j)->y)
			                              });
	}

	// Informs the program that the physical form of an object
	// needs to be recalculated because it has changed.
	recalculateBody = true;
}

void NodeDestructibleRectangle::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draws triangulated triangles
	for (auto& shape : drawableTriangles)
		target.draw(shape, states);
}


void NodeDestructibleRectangle::createBody()
{
	// The physical contour of an object differs from its absolute position of the
	// points inside the game. These must be recalculated. So I create a new vector
	// of point set size for the current object contour.
	b2Vec2* vs{new b2Vec2[polyline.size()]};
	physicalShape = vs;


	// The new physical contour shape
	b2ChainShape Shape;

	// Each of the object's contour points is converted to its physical coordinates matching Box2D.
	for (int i = 0; i < polyline.size(); ++i)
		vs[i].Set(polyline[i]->x / B2_SCALAR, polyline[i]->y / B2_SCALAR);

	// I close the last point with the first to form a kind of polygon.
	Shape.CreateLoop(vs, polyline.size());

	// Prepares the physical properties of an object
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.2f;
	FixtureDef.shape = &Shape;
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::DestructibleGround, *this));
	fixture = Body->CreateFixture(&FixtureDef);
}
