#include "DebugBox2D.h"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"

DebugBox2D::DebugBox2D(sf::RenderWindow& window): window(&window)
{
}

sf::Color DebugBox2D::b2ColorConvert(const b2Color& color, sf::Uint8 alpha)
{
	return sf::Color(static_cast<sf::Uint8>(color.r * 255),
					 static_cast<sf::Uint8>(color.g * 255),
					 static_cast<sf::Uint8>(color.b * 255), 
					 alpha);
}

sf::Vector2f DebugBox2D::b2VecConvert(const b2Vec2& vec)
{
	return sf::Vector2f(vec.x * SCALE, vec.y * SCALE);
}

void DebugBox2D::DrawPolygon(const b2Vec2* vertices, int32 vertices_number, const b2Color& color)
{
	// === Redundancy ALERT ==== //
	// Same as DrawSolidPolygon //

	sf::ConvexShape polygon(vertices_number);
	for (int i = 0; i < vertices_number; ++i)
	{
		sf::Vector2f vec = b2VecConvert(vertices[i]);
		polygon.setPoint(i, vec);
	}
	polygon.setOutlineColor(b2ColorConvert(color));
	polygon.setOutlineThickness(1.f);

	// ======================= //
	polygon.setFillColor(sf::Color::Transparent);
	window->draw(polygon);
}

void DebugBox2D::DrawSolidPolygon(const b2Vec2* vertices, int32 vertices_number, const b2Color& color)
{
	// === Redundancy ALERT ==== //
	// Same as DrawPolygon //

	sf::ConvexShape polygon(vertices_number);
	for (int i = 0; i < vertices_number; ++i)
	{
		sf::Vector2f vec = b2VecConvert(vertices[i]);
		polygon.setPoint(i, vec);
	}
	polygon.setOutlineColor(b2ColorConvert(color));
	polygon.setOutlineThickness(1.f);

	// ======================= //
	polygon.setFillColor(b2ColorConvert(color, 75));
	window->draw(polygon);
}

void DebugBox2D::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	sf::CircleShape circle(radius * SCALE);
	circle.setOrigin(radius * SCALE, radius * SCALE);
	circle.setPosition(b2VecConvert(center));
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(b2ColorConvert(color));

	window->draw(circle);
}

void DebugBox2D::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	sf::CircleShape circle(radius * SCALE);
	circle.setOrigin(radius * SCALE, radius * SCALE);
	circle.setPosition(b2VecConvert(center));
	circle.setFillColor(b2ColorConvert(color, 60));
	circle.setOutlineThickness(1.f);
	circle.setOutlineColor(b2ColorConvert(color));

	b2Vec2 endPoint = center + radius * axis;
	sf::Vertex line[2] =
	{
		sf::Vertex(b2VecConvert(center), b2ColorConvert(color)),
		sf::Vertex(b2VecConvert(endPoint), b2ColorConvert(color)),
	};

	window->draw(circle);
	window->draw(line, 2, sf::Lines);
}

void DebugBox2D::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	// Do it later
}

void DebugBox2D::DrawTransform(const b2Transform& xf)
{
	// Do it later
}

void DebugBox2D::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	// Do it later
}
