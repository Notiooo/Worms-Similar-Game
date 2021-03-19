#include "World.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Nodes/Physical/NodeRectangularPhysical.h"
#include "Nodes/Physical/Specified/Worm/Worm.h"
#include <memory>
#include <array>
#include <random>
#include "Nodes/Physical/CollideTypes.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Bullet.h"

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


	// Set up the round timer
	roundTimeText.setFont(world_fonts.getResourceReference(Fonts_ID::Arial_Narrow));
	roundTimeText.setOutlineThickness(1.f);
	roundTimeText.setPosition(world_view.getSize().x / 2, roundTimeText.getCharacterSize());

	wormQueue.front()->activatePlayState();
	b2_World.SetContactListener(&world_listener);
}

void World::update(sf::Time deltaTime)
{
	// Update the Game World
	root_scene.update(deltaTime);

	// Remove bodies if there are any
	// in the queue to remove
	removeB2Bodies();

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
	// Draw World
	world_window.draw(root_scene);

	// Draw timer
	world_window.draw(roundTimeText);

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

	world_textures.storeResource(Textures_ID::Bazooka, "Resources/Textures/Weapons/bazooka.png");
	world_textures.storeResource(Textures_ID::Bazooka_Bullet, "Resources/Textures/Weapons/bazooka_bullet.png");


	world_fonts.storeResource(Fonts_ID::Arial_Narrow, "Resources/Fonts/arial_narrow.ttf");
}

void World::removeB2Bodies()
{
	while (!NodePhysical::b2_removal_queue.empty())
	{
		b2_World.DestroyBody(NodePhysical::b2_removal_queue.front());
		NodePhysical::b2_removal_queue.pop();
	}
}

void World::createWorld()
{
	// Just some world for testing purposes
	std::unique_ptr<NodeRectangularPhysical> ground = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(640, 50), sf::Vector2f(320, 460), sf::Color::Blue, NodeRectangularPhysical::Physical_Types::Static_Type);
	std::unique_ptr<NodeRectangularPhysical> ramp = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(640, 50), sf::Vector2f(320, 460), sf::Color::Green, NodeRectangularPhysical::Physical_Types::Static_Type);
	std::unique_ptr<NodeRectangularPhysical> box = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(100, 60), sf::Color::Red, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	std::unique_ptr<NodeRectangularPhysical> box2 = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(100, 0), sf::Color::Cyan, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	std::unique_ptr<Worm> worm = std::make_unique<Worm>(b2_World, world_textures, world_fonts, sf::Vector2f(300, 40));
	std::unique_ptr<Worm> worm1 = std::make_unique<Worm>(b2_World, world_textures, world_fonts, sf::Vector2f(380, 40));
	std::unique_ptr<Worm> worm2 = std::make_unique<Worm>(b2_World, world_textures, world_fonts, sf::Vector2f(440, 40));



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
	// On default there is no HideState
	static bool hideState = false;


	sf::Time timeElapsed = roundClock.getElapsedTime();

	if (!hideState && ((timeElapsed > timePerTurn) || (wormQueue.front()->getCurrentState() == State_ID::WormHideState)))
	{
		// Redundancy
		hideState = true;
		if(!(wormQueue.front()->getCurrentState() == State_ID::WormHideState))
			wormQueue.front()->activateHideState();

		roundTimeText.setFillColor(sf::Color::Red);
		roundClock.restart();
		timeElapsed = sf::Time::Zero;
	}

	if (hideState && (timeElapsed > timePerHide))
	{
		Worm* worm = std::move(wormQueue.front());
		worm->activateWaitState();
		wormQueue.pop();
		wormQueue.push(std::move(worm));
		wormQueue.front()->activatePlayState();

		// Redundancy
		hideState = false;
		roundTimeText.setFillColor(sf::Color::White);
		roundClock.restart();
	}

	// Wrong way to do this!!! Too many setString (need to optimize it later)
	if(hideState)
		roundTimeText.setString(std::to_string(static_cast<int>((timePerHide - timeElapsed).asSeconds())));
	else
		roundTimeText.setString(std::to_string(static_cast<int>((timePerTurn - timeElapsed).asSeconds())));

}

void WorldListener::BeginContact(b2Contact* contact)
{
	// Takes information about the occured collision
	b2FixtureUserData userData1 = contact->GetFixtureA()->GetUserData();
	b2FixtureUserData userData2 = contact->GetFixtureB()->GetUserData();

	// All collision informations should be stored as Collision*
	// So I cast it to this object that it supposed to be.
	Collision* node1 = reinterpret_cast<Collision*>(userData1.pointer);
	Collision* node2 = reinterpret_cast<Collision*>(userData2.pointer);

	for (auto& node : { node1, node2 })
	{
		if (node == nullptr)
			continue;

		switch (node->type)
		{
			case CollideTypes::WormFoot:
			{
				// I'm looking for collisions of my worm's feet
				// If it is my worm then it should dynamic_cast
				if (Worm* worm = dynamic_cast<Worm*>(node->object))
					++(worm->footCollisions);
				break;
			}
			
			case CollideTypes::Bullet:
			{
				if (Bullet* bullet = dynamic_cast<Bullet*>(node->object))
					bullet->collision();
				break;
			}
		}
	}
}

void WorldListener::EndContact(b2Contact* contact)
{
	// Takes information about the occured collision
	b2FixtureUserData userData1 = contact->GetFixtureA()->GetUserData();
	b2FixtureUserData userData2 = contact->GetFixtureB()->GetUserData();

	// All collision informations should be stored as NodeScene
	// So I cast it to this object that it supposed to be.
	Collision* node1 = reinterpret_cast<Collision*>(userData1.pointer);
	Collision* node2 = reinterpret_cast<Collision*>(userData2.pointer);

	for (auto& node : { node1, node2 })
	{
		if (node == nullptr)
			continue;

		switch (node->type)
		{
			case CollideTypes::WormFoot:
			{
				// I'm looking for collisions of my worm's feet
				// If it is my worm then it should dynamic_cast
				if (Worm* worm = dynamic_cast<Worm*>(node->object))
					--(worm->footCollisions);
			}
		}
	}
}