#include "pch.h"
#include "Nodes/Physical/NodeDestructibleRectangle.h"

#include <iostream>
#include <random>
#include <clipper/clipper.hpp>
#include <poly2tri/poly2tri.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Nodes/Physical/CollideTypes.h"
#include "Nodes/Physical/NodePhysicalSpark.h"


NodeDestructibleRectangle::NodeDestructibleRectangle(b2World& world, const sf::Texture& texture, sf::Vector2f position, const sf::Vector2f& size):
	NodePhysicalBody(world, Physical_Types::Dynamic_Type, position),
	texture(texture)
{
	triangles.setPrimitiveType(sf::Triangles);
	
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

	// The resulting triangles
	const auto triangulatedFigure = cdt.GetTriangles();

	// The vector with triangles is equal to their number and each triangle has three points
	triangles.resize(triangulatedFigure.size() * 3);

	// I update my vertex set (which builds the figure using triangles) with a new set of triangles obtained by triangulation.
	size_t arrayIndex = 0;
	for (auto* triangulatedTriangle : triangulatedFigure)
	{
		for (size_t vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
		{
			triangles[arrayIndex] = sf::Vertex(
				sf::Vector2f(triangulatedTriangle->GetPoint(vertexIndex)->x, triangulatedTriangle->GetPoint(vertexIndex)->y),
				sf::Vector2f(triangulatedTriangle->GetPoint(vertexIndex)->x, triangulatedTriangle->GetPoint(vertexIndex)->y));
			++arrayIndex;
		}
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

	auto shapeNumber = 0u;
	for (auto& shapes : polyline)
	{
		for (auto& point : shapes)
		{
			// The point building the contour must be transformed in such
			// a way that its absolute position and rotation match.
			const auto transformedPoint = getTransform().transformPoint(static_cast<float>(point->x),
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
	for(auto i = 0; i < polyline.size(); ++i)
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
			const auto transformedPoint = getInverseTransform().transformPoint(static_cast<float>(point.X),
				static_cast<float>(point.Y));
			polyline[shapeNumber][pointNumber++] = new p2t::Point(transformedPoint.x, transformedPoint.y);
		}
		++shapeNumber;
	}

	// I can convert the points obtained in the subtraction process
	// into triangles after processing.
	triangles.clear();

	// More than one figure may have been created as a result of the destruction
	for (auto& shape : polyline)
	{
		p2t::CDT cdt(shape);
		cdt.Triangulate();

		// I get a new set of triangles for the figure currently being processed
		auto triangulatedFigure = cdt.GetTriangles();

		// I update my vertex set (which builds the figure using triangles) with a new set of triangles obtained by triangulation.
		for (auto& triangulatedTriangle : triangulatedFigure)
		{
			for (size_t vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
			{
				triangles.append(sf::Vertex(
					sf::Vector2f(triangulatedTriangle->GetPoint(vertexIndex)->x, triangulatedTriangle->GetPoint(vertexIndex)->y),
					sf::Vector2f(triangulatedTriangle->GetPoint(vertexIndex)->x, triangulatedTriangle->GetPoint(vertexIndex)->y)));
			}
		}
	}
	
	// Informs the program that the physical form of an object
	// needs to be recalculated because it has changed.
	recalculateBody = true;
}

void NodeDestructibleRectangle::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draws triangulated triangles
	states.texture = &texture;
	target.draw(triangles, states);
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
	for (auto shape = 0; shape < polyline.size(); ++shape)
	{
		// The new physical contour shape
		b2ChainShape Shape;

		// Each of the object's contour points is converted to its physical coordinates matching Box2D.
		for (size_t point = 0; point < polyline[shape].size(); ++point)
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
