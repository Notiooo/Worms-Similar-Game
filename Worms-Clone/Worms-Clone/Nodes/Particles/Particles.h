#ifndef PARTICLES_H
#define PARTICLES_H
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

struct Particle
{
	sf::Vector2f position;
	sf::Color color;
	sf::Time lifeTime;
};


#endif