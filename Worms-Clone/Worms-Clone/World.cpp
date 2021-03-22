#include "World.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Nodes/Physical/NodeRectangularPhysical.h"
#include "Nodes/Physical/Specified/Worm/Worm.h"
#include <memory>
#include <array>
#include <random>
#include "Nodes/Physical/CollideTypes.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Bullet.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Hitbox.h"


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

	// Check if there is anything to remove
	root_scene.removeDestroyed();

	// Update the physical world
	b2_World.Step(1 / 60.f, 8, 3);

	checkTurnTime();

	moveScreenWithMouse();
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
	// Zoom in & zoom out the view
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		// Saves coordinates of the mouse on the screen
		const sf::Vector2f oldCoordinates{ world_window.mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y}) };

		// Zoom in & out, and also scales the timer size
		if (event.mouseWheelScroll.delta > 0)
		{
			roundTimeText.setScale(roundTimeText.getScale() / 1.1f);
			world_view.zoom(1.f / 1.1f);
		}
		else
		{
			roundTimeText.setScale(roundTimeText.getScale() * 1.1f);
			world_view.zoom(1.1f);
		}

		// Sets the new view to the window
		world_window.setView(world_view);

		// Reads the "new" position of the mouse (it changed since the zoom is different)
		const sf::Vector2f newCoordinates{ world_window.mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y}) };
		
		// It moves the view, so it will "zoom into" the cursor, and not into the center of the screen
		world_view.move({ oldCoordinates - newCoordinates });
		world_window.setView(world_view);

		// Fixes the posiiton of the timer
		roundTimeText.setPosition(world_window.mapPixelToCoords(sf::Vector2i( world_window.getSize().x / 2, roundTimeText.getCharacterSize())));
	}

	#ifdef _DEBUG
	// Turn off & Turn on drawing the hitboxes
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
	{
		if (debugDraw.GetFlags())
			debugDraw.ClearFlags(b2Draw::e_shapeBit);
		else
			debugDraw.SetFlags(b2Draw::e_shapeBit);
	}
	#endif // DEBUG

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

void World::createWorld()
{
	// Just some world for testing purposes
	std::unique_ptr<NodeRectangularPhysical> ground = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(640, 50), sf::Vector2f(320, 460), sf::Color::Blue, NodeRectangularPhysical::Physical_Types::Static_Type);
	std::unique_ptr<NodeRectangularPhysical> ramp = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(640, 50), sf::Vector2f(320, 460), sf::Color::Green, NodeRectangularPhysical::Physical_Types::Static_Type);
	std::unique_ptr<NodeRectangularPhysical> box = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(100, 60), sf::Color::Red, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	std::unique_ptr<NodeRectangularPhysical> box2 = std::make_unique<NodeRectangularPhysical>(b2_World, sf::Vector2f(20, 20), sf::Vector2f(100, 0), sf::Color::Cyan, NodeRectangularPhysical::Physical_Types::Dynamic_Type);
	std::unique_ptr<Worm> worm = std::make_unique<Worm>(b2_World, world_textures, world_fonts, sf::Vector2f(300, 40), wormQueue);
	std::unique_ptr<Worm> worm1 = std::make_unique<Worm>(b2_World, world_textures, world_fonts, sf::Vector2f(380, 40), wormQueue);
	std::unique_ptr<Worm> worm2 = std::make_unique<Worm>(b2_World, world_textures, world_fonts, sf::Vector2f(440, 40), wormQueue);


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
	if (!wormQueue.empty())
	{

		sf::Time timeElapsed = roundClock.getElapsedTime();

		if (!hideState && ((timeElapsed > timePerTurn) || (wormQueue.front()->getCurrentState() == State_ID::WormHideState)))
		{
			// Redundancy
			hideState = true;
			if (!(wormQueue.front()->getCurrentState() == State_ID::WormHideState))
				wormQueue.front()->activateHideState();

			roundTimeText.setFillColor(sf::Color::Red);
			roundClock.restart();
			timeElapsed = sf::Time::Zero;
		}

		if (hideState && (timeElapsed > timePerHide))
		{
			Worm* worm = std::move(wormQueue.front());
			worm->activateWaitState();
			wormQueue.pop_front();
			wormQueue.push_back(std::move(worm));
			wormQueue.front()->activatePlayState();

			// Redundancy
			hideState = false;
			roundTimeText.setFillColor(sf::Color::White);
			roundClock.restart();
		}

		// Wrong way to do this!!! Too many setString (need to optimize it later)
		if (hideState)
			roundTimeText.setString(std::to_string(static_cast<int>((timePerHide - timeElapsed).asSeconds())));
		else
			roundTimeText.setString(std::to_string(static_cast<int>((timePerTurn - timeElapsed).asSeconds())));
	}
}

void World::moveScreenWithMouse()
{
	static sf::Vector2i oldMouseCoordinates = sf::Mouse::getPosition();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		sf::Vector2i newMouseCoordinates = sf::Mouse::getPosition();
		world_view.move(world_window.mapPixelToCoords(oldMouseCoordinates) - world_window.mapPixelToCoords(newMouseCoordinates));
		world_window.setView(world_view);

		// Fixes the posiiton of the timer
		roundTimeText.setPosition(world_window.mapPixelToCoords(sf::Vector2i(world_window.getSize().x / 2, roundTimeText.getCharacterSize())));
	}
	oldMouseCoordinates = sf::Mouse::getPosition();
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
					bullet->setDestroyed();
				break;
			}

			case CollideTypes::Hitbox:
			{
				if (node1 == nullptr || node2 == nullptr)
					break;
				
				Collision* worm_collision = (node1->type == CollideTypes::WormBody) ? node1 : node2;
				Hitbox* hitbox = dynamic_cast<Hitbox*>(node->object);
				if (Worm* worm = dynamic_cast<Worm*>(worm_collision->object))
				{
					worm->activateHitState();

					// First I calculate distance from the explosion
					sf::Vector2f distance_from_explosion = (hitbox->getAbsolutePosition() - worm->getAbsolutePosition());
					
					// Then I had to take under consideration that object closer (with smaller distance) should be pushed further
					float range_of_hitbox = hitbox->area_of_range / 2.f / NodePhysical::B2_SCALAR;

					sf::Vector2f force_vector;
					force_vector.x = (distance_from_explosion.x) ? (range_of_hitbox - distance_from_explosion.x) : -(range_of_hitbox + distance_from_explosion.x);
					force_vector.y = (distance_from_explosion.y) ? (range_of_hitbox - distance_from_explosion.y) : -(range_of_hitbox + distance_from_explosion.y);

					// The force has to be negative to push AWAY from the explosion
					worm->applyForce(force_vector * 300.f);
					worm->setDamage(hitbox->max_dmg * (std::sqrt(force_vector.x * force_vector.x + force_vector.y * force_vector.y)) / range_of_hitbox);
				}

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
				break;
			}
		}
	}
}