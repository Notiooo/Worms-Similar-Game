#include "TestDestructibleNode.h"


#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "clipper.hpp"
#include "poly2tri.h"


TestDestructibleNode::TestDestructibleNode(const sf::Texture& texture)
{

	// Create examplary triangle 
	nodeShape.setTexture(&texture);
	nodeShape.setPointCount(4);
	nodeShape.setPoint(0, { 0, 0 });
	nodeShape.setPoint(1, { 0, 200 });
	nodeShape.setPoint(2, { 200, 200 });
	nodeShape.setPoint(3, {200, 0});
	nodeShape.setTextureRect(sf::IntRect{ 0, 0, static_cast<int>(nodeShape.getGlobalBounds().width), static_cast<int>(nodeShape.getGlobalBounds().height) });


	// Convert it to the Clipper
	ClipperLib::Paths shape(1), clip(1), solution(1);
	for (size_t i = 0; i < nodeShape.getPointCount(); ++i)
		shape[0].emplace_back(ClipperLib::IntPoint(nodeShape.getPoint(i).x, nodeShape.getPoint(i).y));

	// Exemplary shape to differentate it
	double radius = 250.f;
	float centerX = 250.f;
	float centerY = 250.f;
	for (double angle = 0; angle <= 2 * 3.14159; angle += 0.1)
		clip[0].emplace_back(ClipperLib::IntPoint(centerX + radius * std::cos(angle), centerY + radius * std::sin(angle)));

	ClipperLib::Clipper c;
	c.AddPath(shape[0], ClipperLib::ptSubject, true);
	c.AddPath(clip[0], ClipperLib::ptClip, true);
	c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

	// After differentiation change current values of the triangle
	nodeShape.setPointCount(solution[0].size());
	for (size_t i = 0; i < nodeShape.getPointCount(); ++i)
		nodeShape.setPoint(i, sf::Vector2f{ static_cast<float>(solution[0].at(i).X), static_cast<float>(solution[0].at(i).Y) });

	// Ad them also to the vector of poly2tri
	for(size_t i = 0; i < nodeShape.getPointCount(); ++i)
		polyline.push_back(new p2t::Point(nodeShape.getPoint(i).x, nodeShape.getPoint(i).y));

	// Triangulate
	p2t::CDT cdt(polyline);
	cdt.Triangulate();

	// Triangles put into the vector of triangles to draw
	triangles = cdt.GetTriangles();
	for (auto& triangle : triangles)
	{
		shapies.emplace_back(3);
		for(int i = 0; i < 3; ++i)
			shapies.back().setPoint(i, sf::Vector2f{ static_cast<float>(triangle->GetPoint(i)->x), static_cast<float>(triangle->GetPoint(i)->y) });
	}

}

TestDestructibleNode::~TestDestructibleNode()
{
	for (int i = 0; i < polyline.size(); ++i)
		delete polyline[i];
}

void TestDestructibleNode::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw triangulated triangles
	for(auto& shape : shapies)
	{
		target.draw(shape, states);
	}
}
