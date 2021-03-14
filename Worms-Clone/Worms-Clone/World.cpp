#include "World.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Nodes/Physical/NodeRectangularPhysical.h"
#include "Nodes/Physical/Specified/Worm/Worm.h"
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

	#ifdef _DEBUG
	// set flag to draw the debug shapes
	debugDraw.SetFlags(b2Draw::e_shapeBit);
	#endif

	loadResources();
	createWorld();

	wormQueue.front()->activatePlayState();
}

void World::update(sf::Time deltaTime)
{
	// Update the Game World
	root_scene.update(deltaTime);

	// Update the physical world
	b2_World.Step(1 / 60.f, 8, 3);

	checkTurnTime();
}

void World::Box2DdrawDebug()
{
	// Draw the debug part of physical objects
	b2_World.DebugDraw();
}

void World::draw() const
{
	world_window.draw(root_scene);

	// Some hack as Box2D drawDebug is non-const
	const_cast<World*>(this)->Box2DdrawDebug();
}

void World::processEvents(const sf::Event& event)
{
	// == Just for fun, for remove later == //
	// But it allows to create randomized-colored rectangles by clicking with mouse button
	
	// The array of possible collors
	static std::array<sf::Color, 6> colors = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Cyan, sf::Color::Magenta, sf::Color::Yellow };
	
	// The random engine
	static std::default_random_engine e;
	static std::uniform_int_distribution<unsigned> u(0, colors.size() - 1);

	// The spawning and pinning to the root scene
	if (event.type == sf::Event::MouseButtonPressed)
	{
		std::unique_ptr<NodeRectangularPhysical> box = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(sf::Mouse::getPosition(world_window)), colors[u(e)], NodeRectangularPhysical::Physical_Types::Dynamic_Type);
		root_scene.pinNode(std::move(box));
	}

	// Pass the handling to the scene hierarchy
	root_scene.handleEvents(event);
}

void World::loadResources()
{
	// will load some later
	world_textures.storeResource(Textures_ID::AnExamplaryWorm, "Resources/Textures/An_example_worm.png");
	world_textures.storeResource(Textures_ID::Rope, "Resources/Textures/World/rope.png");
	world_textures.getResourceReference(Textures_ID::Rope).setRepeated(true);


}

void World::createWorld()
{
	// Just some world for testing purposes
	std::unique_ptr<NodeRectangularPhysical> ground = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(640, 50), sf::Vector2f(320, 460), sf::Color::Blue, NodeRectangularPhysical::Physical_Types::Static_Type);
	std::unique_ptr<NodeRectangularPhysical> ramp = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(640, 50), sf::Vector2f(320, 460), sf::Color::Green, NodeRectangularPhysical::Physical_Types::Static_Type);
	std::unique_ptr<NodeRectangularPhysical> box = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(320, 60), sf::Color::Red, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	std::unique_ptr<NodeRectangularPhysical> box2 = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(325, 0), sf::Color::Cyan, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	std::unique_ptr<Worm> worm = std::make_unique<Worm>(b2_World, world_textures, sf::Vector2f(300, 40));
	std::unique_ptr<Worm> worm1 = std::make_unique<Worm>(b2_World, world_textures, sf::Vector2f(300, 40));
	std::unique_ptr<Worm> worm2 = std::make_unique<Worm>(b2_World, world_textures, sf::Vector2f(300, 40));



	wormQueue.push(worm.get());
	wormQueue.push(worm1.get());
	wormQueue.push(worm2.get());



	ramp->setRotation(40);
	root_scene.pinNode(std::move(ground));
	root_scene.pinNode(std::move(ramp));

	root_scene.pinNode(std::move(worm));
	root_scene.pinNode(std::move(worm1));
	root_scene.pinNode(std::move(worm2));
	root_scene.pinNode(std::move(box));
	root_scene.pinNode(std::move(box2));
}

void World::checkTurnTime()
{
	// Switch Worm if turn has ended
	static sf::Clock timeRound;
	if (!hideState && (timeRound.getElapsedTime() > timePerTurn))
	{
		wormQueue.front()->activateHideState();
		hideState = true;
		timeRound.restart();
	}

	if (hideState && (timeRound.getElapsedTime() > timePerHide))
	{
		Worm* worm = std::move(wormQueue.front());
		worm->activateWaitState();
		wormQueue.pop();
		wormQueue.push(std::move(worm));

		wormQueue.front()->activatePlayState();
		hideState = false;
		timeRound.restart();
	}
}
