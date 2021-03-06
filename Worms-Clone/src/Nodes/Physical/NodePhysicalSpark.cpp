#include "pch.h"
#include "Nodes/Physical/NodePhysicalSpark.h"

#include <random>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

NodePhysicalSpark::NodePhysicalSpark(b2World& world, SoundPlayer& sounds, const sf::Texture& smokeTexture, sf::Vector2f position, sf::Color color) :
	NodePhysicalBase(world),
	sparkColor(color),
	smokeParticles(smokeTexture),
	soundPlayer(sounds)
{
	soundPlayer.play(Sound_ID::Explosion, position);
	
	smokeParticles.setFadingOut(sf::seconds(timeToDelete.asSeconds()));
	smokeParticles.setDefaultOpacity(0.05f);
	smokeParticles.setParticleColor(color);
	//smokeParticles.setParticleLifeTime(sf::seconds(timeToDelete.asSeconds() * 2.f));
	smokeParticles.setParticleInfiniteLifeTime(true);
	
	smokeParticles.setFadingLasts(sf::seconds(timeToDelete.asSeconds()/ 6.f));
	
	// It will be used to create randomized-velocity sparks
	static std::default_random_engine e(time(0));
	static std::uniform_real_distribution<float> g(0.f, 4.f);

	// Creates graphical-side of sparks
	for (auto& spark : particles)
	{
		spark.shape.setFillColor(sparkColor);
		spark.shape.setSize({10.f, 10.f});
		spark.smokeEmitter = std::make_unique<NodeEmitter>(smokeParticles);
	}

	// Create physical-side of sparks
	const int number_of_particles = particles.size();
	for (int i = 0; i < number_of_particles; ++i)
	{
		// All sparks face around (360) the object.
		float angle = angleToRadians((i / static_cast<float>(number_of_particles)) * 360);
		b2Vec2 rayDir(std::sin(angle), std::cos(angle));

		// Definition of the body
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.position = sfVectorToB2Vec(position);
		bd.linearVelocity = (7 + g(e)) * rayDir;
		particles[i].body = World->CreateBody(&bd);

		// Definiton of circular shape of the body
		b2CircleShape circleShape;
		circleShape.m_radius = 0.05;
		b2FixtureDef fd;
		fd.shape = &circleShape;
		fd.density = 1.f;
		fd.restitution = 0.3f;
		fd.filter.groupIndex = -1;
		particles[i].body->CreateFixture(&fd);
	}
}

NodePhysicalSpark::~NodePhysicalSpark()
{
	for (auto& spark : particles)
		World->DestroyBody(spark.body);
}

void NodePhysicalSpark::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{	
	// Sparks are placed in the world independently
	// Thats why I'm not using the "states" which also includes
	// the transform of the Node
	for (const auto& spark : particles)
		target.draw(spark.shape);

	target.draw(smokeParticles);
}

void NodePhysicalSpark::updateThis(sf::Time deltaTime)
{
	if (clock.getElapsedTime() > timeToDelete)
		setDestroyed();

	smokeParticles.update(deltaTime);

	for (auto& spark : particles)
		spark.smokeEmitter->update(deltaTime);
}

void NodePhysicalSpark::updatePhysics()
{
	for (auto& spark : particles)
	{
		// Set position of particular spark
		spark.shape.setPosition(b2VecToSfVector<sf::Vector2f>(spark.body->GetPosition()));

		// Fading effect
		sparkColor.a = 255 - 255 * (clock.getElapsedTime().asSeconds() / timeToDelete.asSeconds());
		spark.shape.setFillColor(sparkColor);

		// Smoke effect
		spark.smokeEmitter->setPosition(spark.shape.getPosition());
	}
}
