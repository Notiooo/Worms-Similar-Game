#include "NodePhysicalSpark.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include <random>

NodePhysicalSpark::NodePhysicalSpark(b2World& world, sf::Vector2f position):
	NodePhysical(world, NodePhysical::Physical_Types::Static_Type, position)
{
	static std::default_random_engine e(time(0));
	static std::uniform_real_distribution<float> g(0.f, 4.f);
	for (auto& pair : particles)
	{
		// Color doesnt matter yet
		pair.first.setFillColor(sf::Color::White);

		pair.first.setSize({ 10.f, 10.f });
	}

	int number_of_particles = particles.size();
	for (int i = 0; i < number_of_particles; ++i)
	{
		float angle = angleToRadians((i / static_cast<float>(number_of_particles)) * 360);
		b2Vec2 rayDir(std::sin(angle), std::cos(angle));

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.position = sfVector_to_b2Vec(position); 
		bd.linearVelocity = (7 + g(e)) * rayDir;
		particles[i].second = World->CreateBody(&bd);

		b2CircleShape circleShape;
		circleShape.m_radius = 0.05;

		b2FixtureDef fd;
		fd.shape = &circleShape;
		fd.density = 1.f;
		fd.restitution = 0.3f;
		fd.filter.groupIndex = -1;
		particles[i].second->CreateFixture(&fd);
	}
}

NodePhysicalSpark::~NodePhysicalSpark()
{
	for (auto& pair : particles)
		World->DestroyBody(pair.second);
}

void NodePhysicalSpark::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& pair : particles)
		target.draw(pair.first, states);
}

void NodePhysicalSpark::updateThis(sf::Time deltaTime)
{
	if (clock.getElapsedTime() > time_to_delete)
		setDestroyed();


	for (auto& pair : particles)
	{
		pair.first.setPosition(b2Vec_to_sfVector<sf::Vector2f>(pair.second->GetPosition()));

		// Fading effect
		pair.first.setFillColor(sf::Color(255, 255, 255, 255 - 255 * (clock.getElapsedTime().asSeconds() / time_to_delete.asSeconds())));
	}
}