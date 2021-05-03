#include "NodeDestructibleRectangle.h"


#include <iostream>
#include <random>
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
	polyline.resize(1);
	polyline[0].push_back(new p2t::Point(0 - originTransform.x, 0 - originTransform.y));
	polyline[0].push_back(new p2t::Point(size.x - originTransform.x, 0 - originTransform.y));
	polyline[0].push_back(new p2t::Point(size.x - originTransform.x, size.y - originTransform.y));
	polyline[0].push_back(new p2t::Point(0 - originTransform.x, size.y - originTransform.y));


	// A triangulation process that transforms the outline/shape of an object into triangles.
	p2t::CDT cdt(polyline[0]);
	cdt.Triangulate();

	// The resulting triangles are written in the vector
	triangles.emplace_back(cdt.GetTriangles());

	// The vector with graphical triangles is equal to their number and each triangle has three points
	drawableTriangles.resize(1);
	drawableTriangles[0].resize(triangles[0].size(), sf::ConvexShape(3));

	// Each graphical triangle is assigned the coordinates of the calculated triangles obtained from the triangulation process.
	for (size_t i = 0; i < triangles[0].size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
			drawableTriangles[0][i].setPoint(j, sf::Vector2f{
				                              static_cast<float>(triangles[0][i]->GetPoint(j)->x),
				                              static_cast<float>(triangles[0][i]->GetPoint(j)->y)
			                              });
	}

	// Finally the physical body can be made
	createBody();
}

NodeDestructibleRectangle::~NodeDestructibleRectangle()
{
	// I remove points that build the outline of an object
	for (auto& shape : polyline)
		for(auto& point : shape)
			delete point;
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

		for (auto& fixture : fixtures)
			Body->DestroyFixture(fixture);

		physicalShape.clear();

		// And create new body
		createBody();
		recalculateBody = false;
	}
}

void NodeDestructibleRectangle::addHole(std::vector<ClipperLib::IntPoint>& figure)
{
	// First I will have to transform both figures in such a way that Clipper
	// can subtract them from each other.
	ClipperLib::Paths shape(polyline.size()), clip(1), solution(1);

	unsigned shapeNumber = 0;
	for (auto& shapes : polyline)
	{
		for (auto& point : shapes)
		{
			// The point building the contour must be transformed in such
			// a way that its absolute position and rotation match.
			auto transformedPoint = getTransform().transformPoint(static_cast<float>(point->x),
				static_cast<float>(point->y));
			shape[shapeNumber].emplace_back(ClipperLib::IntPoint(transformedPoint.x, transformedPoint.y));
		}
		++shapeNumber;
	}

	// The figure should already be processed, so there is no problem with it
	for (auto& point : figure)
		clip[0].emplace_back(point);

	// I then carry out the operations to cut the object
	ClipperLib::Clipper c;
	for(int i = 0; i < polyline.size(); ++i)
		c.AddPath(shape[i], ClipperLib::ptSubject, true);
	c.AddPath(clip[0], ClipperLib::ptClip, true);
	c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

	// After differentiation I remove current values of the triangle
	for (auto& shape : polyline)
		for (auto& point : shape)
			delete point;
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
	polyline.resize(solution.size());
	for (size_t i = 0; i < solution.size(); ++i)
		polyline[i].resize(solution[i].size());

	// Then I process all the points calculated by Clipper
	shapeNumber = 0;
	for (auto& shape : solution)
	{
		unsigned pointNumber = 0;
		for (auto& point : shape)
		{
			// I process the points obtained so that they are again independent of absolute position
			// and rotation -- that is, I calculate the relativistic position for that object.
			auto transformedPoint = getInverseTransform().transformPoint(static_cast<float>(point.X),
				static_cast<float>(point.Y));
			polyline[shapeNumber][pointNumber++] = new p2t::Point(transformedPoint.x, transformedPoint.y);
		}
		++shapeNumber;
	}

	// I can convert the points obtained in the subtraction process
	// into triangles after processing.
	drawableTriangles.resize(polyline.size());
	triangles.resize(polyline.size());
	for (size_t shape = 0; shape < polyline.size(); ++shape)
	{
		p2t::CDT cdt(polyline[shape]);
		cdt.Triangulate();

		// Updates the set of triangles building this object.
		triangles[shape] = cdt.GetTriangles();

		// Again, it prepares a set of drawable triangles whose size
		// corresponds to the number of resulting triangles consisting
		// of (no surprise) three points
		drawableTriangles[shape].resize(triangles[shape].size(), sf::ConvexShape(3));


		// Each visual triangle has assigned points equal to those
		// obtained after the triangulation process.
		for (size_t i = 0; i < triangles[shape].size(); ++i)
		{
			for (int j = 0; j < 3; ++j)
				drawableTriangles[shape][i].setPoint(j, sf::Vector2f{
												  static_cast<float>(triangles[shape][i]->GetPoint(j)->x),
												  static_cast<float>(triangles[shape][i]->GetPoint(j)->y)
					});
		}
	}
	
	// Informs the program that the physical form of an object
	// needs to be recalculated because it has changed.
	recalculateBody = true;
}

void NodeDestructibleRectangle::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draws triangulated triangles
	for (auto& shape : drawableTriangles)
		for(auto& triangle : shape)
			target.draw(triangle, states);
}


void NodeDestructibleRectangle::createBody()
{
	// The physical contour of an object differs from its absolute position of the
	// points inside the game. These must be recalculated. So I create a new vector
	// of point set size for the current object contour.
	physicalShape.resize(polyline.size());
	
	for(size_t shape = 0; shape < polyline.size(); ++shape)
	{
		physicalShape[shape] = std::unique_ptr<b2Vec2[], PhysicalShapeDeleter>(new b2Vec2[polyline[shape].size()], PhysicalShapeDeleter());
	}

	fixtures.resize(polyline.size());
	for (int shape = 0; shape < polyline.size(); ++shape)
	{
		// The new physical contour shape
		b2ChainShape Shape;

		// Each of the object's contour points is converted to its physical coordinates matching Box2D.
		for (int point = 0; point < polyline[shape].size(); ++point)
			physicalShape[shape][point].Set(polyline[shape][point]->x / B2_SCALAR, polyline[shape][point]->y / B2_SCALAR);

		// I close the last point with the first to form a kind of polygon.
		Shape.CreateLoop(physicalShape[shape].get(), polyline[shape].size());

		// Prepares the physical properties of an object
		b2FixtureDef FixtureDef;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.7f;
		FixtureDef.restitution = 0.2f;
		FixtureDef.shape = &Shape;
		FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::DestructibleGround, *this));
		fixtures[shape] = Body->CreateFixture(&FixtureDef);
	}
}
