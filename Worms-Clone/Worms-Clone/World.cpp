#include "World.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Nodes/Physical/NodeRectangularPhysical.h"
#include <memory>
#include <array>
#include <random>

World::World(sf::RenderWindow& window) :
world_window(window),
world_view(window.getDefaultView()),
b2_World(b2Vec2(0.f, 9.8f)),
debugDraw(window)
{
	// Tells the physical world what to draw
	b2_World.SetDebugDraw(&debugDraw);

	// draw only shapes
	debugDraw.SetFlags(b2Draw::e_shapeBit);
	

	loadResources();
	createWorld();
}

void World::update(sf::Time deltaTime)
{
	root_scene.update(deltaTime);
	b2_World.Step(1 / 60.f, 8, 3);
}

void World::draw()
{
	world_window.draw(root_scene);

	// Draw the debug part of physical objects
	b2_World.DebugDraw();
}

void World::processEvents(const sf::Event& event)
{
	// == Just for fun, for remove later == //
	static std::array<sf::Color, 6> colors = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Cyan, sf::Color::Magenta, sf::Color::Yellow };
	static std::default_random_engine e;
	static std::uniform_int_distribution<unsigned> u(0, colors.size() - 1);

	if (event.type == sf::Event::MouseButtonPressed)
	{
		std::unique_ptr<NodeRectangularPhysical> box = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(sf::Mouse::getPosition(world_window)), colors[u(e)], NodeRectangularPhysical::Physical_Types::Dynamic_Type);
		root_scene.pinNode(std::move(box));
	}
}

void World::loadResources()
{
	// will load some later
	//world_textures.storeResource(Textures_ID::AnExamplaryWorm, "Resources/Textures/An_example_worm.png");

}

void World::createWorld()
{
	std::unique_ptr<NodeRectangularPhysical> ground = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(640, 50), sf::Vector2f(320, 460), sf::Color::Blue, NodeRectangularPhysical::Physical_Types::Static_Type);
	std::unique_ptr<NodeRectangularPhysical> box = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(320, 60), sf::Color::Red, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	std::unique_ptr<NodeRectangularPhysical> box2 = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(325, 0), sf::Color::Cyan, NodeRectangularPhysical::Physical_Types::Dynamic_Type);

	root_scene.pinNode(std::move(ground));
	root_scene.pinNode(std::move(box));
	root_scene.pinNode(std::move(box2));




}
