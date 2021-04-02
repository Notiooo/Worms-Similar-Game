#ifndef NODEPHYSICALSPARK_H
#define NODEPHYSICALSPARK_H

#include "NodePhysicalBody.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"
#include <array>


/**
 * \brief An object that creates tiny objects that escape all around from the place of this object.
 * The object is removed when they are created.
 *
 * It is used to graphically visualize explosions of various objects.
 */
class NodePhysicalSpark : public NodePhysicalBase
{
public:
	NodePhysicalSpark(b2World& world, sf::Vector2f position, sf::Color color = sf::Color::White);
	~NodePhysicalSpark() override;

	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;
	
	/**
	 * \brief It synchronises all created particles with their position inside the physical simulation.
	 */
	void updatePhysics() override;

private:


	/**
	 * \brief A structure describing a single particle.
	 */
	struct Sparkle
	{
		sf::RectangleShape shape;
		b2Body* body;
	};

	std::array<Sparkle, 40> particles; //!<  It stores all the particles created. Their number is constantly 40.
	sf::Color sparkColor; //!< Color of the particle

	sf::Clock clock; //!< A timer measuring time for the removal of particles.
	sf::Time timeToDelete = sf::seconds(5); //!< Time after which the particles are deleted
};


#endif