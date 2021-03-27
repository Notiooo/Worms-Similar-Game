#ifndef NODEPHYSICALSPARK_H
#define NODEPHYSICALSPARK_H

#include "NodePhysicalBody.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"
#include <array>


class NodePhysicalSpark : public NodePhysicalBase
{
public:
	NodePhysicalSpark(b2World& world, sf::Vector2f position, sf::Color color = sf::Color::White);
	~NodePhysicalSpark();

	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;
	void updatePhysics();

private:
	struct Sparkle
	{
		sf::RectangleShape shape;
		b2Body* body;
	};
	std::array<Sparkle, 40> particles;
	sf::Clock clock;
	sf::Time timeToDelete = sf::seconds(5);

	sf::Color sparkColor;
};


#endif