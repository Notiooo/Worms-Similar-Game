#include "TestDestructibleNode.h"


#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "clipper.hpp"
#include "poly2tri.h"
#include "../utils.h"
#include "Physical/CollideTypes.h"


TestDestructibleNode::TestDestructibleNode(b2World& world, Physical_Types physical_type,
                                           sf::Vector2f position, const sf::Vector2f& size):
NodePhysicalBody(world, physical_type, position)
{
	originTransform = { size.x / 2.f, size.y / 2.f };
	
	polyline.push_back(new p2t::Point(0 - originTransform.x, 0 - originTransform.y));
	polyline.push_back(new p2t::Point(size.x - originTransform.x, 0 - originTransform.y));
	polyline.push_back(new p2t::Point(size.x - originTransform.x, size.y - originTransform.y));
	polyline.push_back(new p2t::Point(0 - originTransform.x, size.y - originTransform.y));


	// Triangulate
	p2t::CDT cdt(polyline);
	cdt.Triangulate();

	// Triangles put into the vector of triangles to draw
	triangles = cdt.GetTriangles();
	shapies.resize(triangles.size(), sf::ConvexShape(3));
	for (size_t i = 0; i < triangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
			shapies[i].setPoint(j, sf::Vector2f{ static_cast<float>(triangles[i]->GetPoint(j)->x), static_cast<float>(triangles[i]->GetPoint(j)->y) });
	}

	createBody();
}

TestDestructibleNode::~TestDestructibleNode()
{
	for (int i = 0; i < polyline.size(); ++i)
		delete polyline[i];

	delete[] physicalShape;
	physicalShape = nullptr;
}

void TestDestructibleNode::updateThis(sf::Time deltaTime)
{
	// Synchronize the drawable Rectangle with the physical object
	updatePhysics();

	if(recalculateBody)
	{
		for (b2Fixture* fix = Body->GetFixtureList(); fix; fix = fix->GetNext())
		{
			delete reinterpret_cast<Collision*>(fix->GetUserData().pointer);
		}
		delete[] physicalShape;
		physicalShape = nullptr;
		Body->DestroyFixture(fixture);
		createBody();
		recalculateBody = false;
	}
}

void TestDestructibleNode::addHole(std::vector<ClipperLib::IntPoint>& figure)
{
	// Convert it to the Clipper
	ClipperLib::Paths shape(1), clip(1), solution(1);
	for (auto& point : polyline)
		shape[0].emplace_back(ClipperLib::IntPoint(getTransform().transformPoint(sf::Vector2f{ static_cast<float>(point->x), static_cast<float>(point->y) }).x, getTransform().transformPoint(sf::Vector2f{ static_cast<float>(point->x), static_cast<float>(point->y) }).y));

	for (auto& point : figure)
		clip[0].emplace_back(point);

	ClipperLib::Clipper c;
	c.AddPath(shape[0], ClipperLib::ptSubject, true);
	c.AddPath(clip[0], ClipperLib::ptClip, true);
	c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

	// After differentiation change current values of the triangle
	for (int i = 0; i < polyline.size(); ++i)
		delete polyline[i];
	polyline.clear();


	// If there are no points it means it got completely destroyed
	if(solution.empty())
	{
		setDestroyed();
		return;
	}

	polyline.resize(solution[0].size());
	for (size_t i = 0; i < solution[0].size(); ++i)
		polyline[i] = new p2t::Point(getInverseTransform().transformPoint(sf::Vector2f{ static_cast<float>(solution[0][i].X), static_cast<float>(solution[0][i].Y) }).x, 
			getInverseTransform().transformPoint(sf::Vector2f{ static_cast<float>(solution[0][i].X), static_cast<float>(solution[0][i].Y) }).y);

	// Triangulate
	p2t::CDT cdt(polyline);
	cdt.Triangulate();

	// Triangles put into the vector of triangles to draw
	triangles = cdt.GetTriangles();
	shapies.resize(triangles.size(), sf::ConvexShape(3));
	for (size_t i = 0; i < triangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
			shapies[i].setPoint(j, sf::Vector2f{ static_cast<float>(triangles[i]->GetPoint(j)->x), static_cast<float>(triangles[i]->GetPoint(j)->y) });
	}

	recalculateBody = true;
}

void TestDestructibleNode::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw triangulated triangles
	for(auto& shape : shapies)
	{
		target.draw(shape, states);
	}
}


void TestDestructibleNode::createBody()
{
	// Defines its shape
	b2Vec2* vs{ new b2Vec2[polyline.size()] };
	physicalShape = vs;

	b2ChainShape Shape;

	// Chain Shape Fixture
	for (int i = 0; i < polyline.size(); ++i)
		vs[i].Set(polyline[i]->x / B2_SCALAR, polyline[i]->y / B2_SCALAR);
	Shape.CreateLoop(vs, polyline.size());

	b2FixtureDef FixtureDef;
	
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.2f;
	FixtureDef.shape = &Shape;
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::DestructibleGround, *this));
	fixture = Body->CreateFixture(&FixtureDef);
}