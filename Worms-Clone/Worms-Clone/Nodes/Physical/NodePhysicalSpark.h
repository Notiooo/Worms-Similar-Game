#ifndef NODEPHYSICALSPARK_H
#define NODEPHYSICALSPARK_H

#include "NodePhysical.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"
#include <array>


class NodePhysicalSpark : public NodePhysical
{
public:
	NodePhysicalSpark(b2World& world, sf::Vector2f position);
	~NodePhysicalSpark();

	virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateThis(sf::Time deltaTime) override;

private:
	std::array<std::pair<sf::RectangleShape, b2Body*>, 40> particles;
	sf::Clock clock;
	sf::Time time_to_delete = sf::seconds(5);
};


#endif