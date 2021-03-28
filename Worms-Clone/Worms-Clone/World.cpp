#include "World.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Nodes/Physical/Specified/Worm/Worm.h"
#include <memory>
#include "Nodes/Physical/Specified/Worm/Weapons/Bullet.h"
#include <fstream>
#include <sstream>
#include <iostream>


World::World(sf::RenderWindow& window) :
	worldWindow(window),
	worldView(window.getDefaultView()),
	b2_World(b2Vec2(0.f, 9.8f)),
	debugDraw(window),
	essentials({&b2_World, &worldWindow, &worldTextures, &worldFonts})
{
	// Tells the physical world what to draw
	b2_World.SetDebugDraw(&debugDraw);

#ifdef _DEBUG
	// set flag to draw the debug shapes
	debugDraw.SetFlags(b2Draw::e_shapeBit);
#endif


	loadResources();
	createWorld();

	backgroundSprite.setTexture(worldTextures.getResourceReference(Textures_ID::WorldBackground));
	backgroundSprite.setTextureRect(sf::IntRect(0, 0, worldView.getSize().x, worldView.getSize().y));


	// Set up the round timer
	roundTimeText.setFont(worldFonts.getResourceReference(Fonts_ID::ArialNarrow));
	roundTimeText.setOutlineThickness(1.f);
	roundTimeText.setPosition(worldView.getSize().x / 2, roundTimeText.getCharacterSize());

	wormQueue.front()->activateState(State_ID::WormPlayState);
	
	b2_World.SetContactListener(&worldListener);
}

void World::update(sf::Time deltaTime)
{
	// Update the Game World
	rootScene.update(deltaTime);

	// Check if there is anything to remove
	rootScene.removeDestroyed();

	// Update the physical world
	b2_World.Step(1 / 60.f, 8, 3);

	checkTurnTime();

	moveScreenWithMouse();
}


void World::box2DdrawDebug()
{
	// Draw the debug part of physical objects
	b2_World.DebugDraw();
}

void World::draw() const
{
	// Draw Background
	worldWindow.draw(backgroundSprite);

	// Draw World
	worldWindow.draw(rootScene);

	// Draw timer
	worldWindow.draw(roundTimeText);

	// Some hack as Box2D drawDebug is non-const
	const_cast<World*>(this)->box2DdrawDebug();
}

void World::processEvents(const sf::Event& event)
{
	// Zoom in & zoom out the view
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		// Saves coordinates of the mouse on the screen
		const sf::Vector2f oldCoordinates{
			worldWindow.mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y})
		};

		// Zoom in & out, and also scales the timer size
		auto current_zoom = worldView.getSize().x;
		auto maximum_zoom = worldWindow.getDefaultView().getSize().x * maxZoomFactor;
		auto minimum_zoom = worldWindow.getDefaultView().getSize().x / maxZoomFactor;

		// Zooming in
		if (event.mouseWheelScroll.delta > 0)
		{
			if (current_zoom > minimum_zoom)
			{
				roundTimeText.setScale(roundTimeText.getScale() / 1.1f);
				worldView.zoom(1.f / 1.1f);
			}
		}
		// Zooming out
		else
		{
			if (current_zoom < maximum_zoom)
			{
				roundTimeText.setScale(roundTimeText.getScale() * 1.1f);
				worldView.zoom(1.1f);
			}
		}

		// Sets the new view to the window
		worldWindow.setView(worldView);

		// Reads the "new" position of the mouse (it changed since the zoom is different)
		const sf::Vector2f newCoordinates{
			worldWindow.mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y})
		};

		// It moves the view, so it will "zoom into" the cursor, and not into the center of the screen
		worldView.move({oldCoordinates - newCoordinates});
		worldWindow.setView(worldView);

		// Fixes the posiiton of the timer
		roundTimeText.setPosition(
			worldWindow.mapPixelToCoords(sf::Vector2i(worldWindow.getSize().x / 2,
			                                          roundTimeText.getCharacterSize())));

		// Fixes the background
		backgroundSprite.setTextureRect(sf::IntRect(0, 0, worldView.getSize().x, worldView.getSize().y));
		backgroundSprite.setPosition(worldWindow.mapPixelToCoords({0, 0}));
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
	rootScene.handleEvents(event);
}

void World::loadResources()
{
	worldTextures.storeResource(Textures_ID::WorldBackground, "Resources/Textures/World/background_texture.png");
	worldTextures.getResourceReference(Textures_ID::WorldBackground).setRepeated(true);
	worldTextures.storeResource(Textures_ID::Paper, "Resources/Textures/World/paper_texture.png");
	worldTextures.getResourceReference(Textures_ID::Paper).setRepeated(true);

	// will load some later
	worldTextures.storeResource(Textures_ID::AnExemplaryWorm, "Resources/Textures/An_example_worm.png");
	worldTextures.storeResource(Textures_ID::HitWorm, "Resources/Textures/Worm_Hit.png");
	worldTextures.storeResource(Textures_ID::DeadWorm, "Resources/Textures/Dead_Worm.png");
	worldTextures.storeResource(Textures_ID::Rope, "Resources/Textures/World/rope.png");
	worldTextures.getResourceReference(Textures_ID::Rope).setRepeated(true);

	worldTextures.storeResource(Textures_ID::Bazooka, "Resources/Textures/Weapons/bazooka.png");
	worldTextures.storeResource(Textures_ID::BazookaThumbnail, "Resources/Textures/Weapons/bazooka_thumbnail.png");
	worldTextures.storeResource(Textures_ID::BazookaBullet, "Resources/Textures/Weapons/bazooka_bullet.png");
	worldTextures.storeResource(Textures_ID::Inventory, "Resources/Textures/Weapons/Inventory/background.png");

	worldTextures.storeResource(Textures_ID::Cannon, "Resources/Textures/Weapons/cannon.png");
	worldTextures.storeResource(Textures_ID::CannonThumbnail, "Resources/Textures/Weapons/cannon_thumbnail.png");
	worldTextures.storeResource(Textures_ID::CannonBullet, "Resources/Textures/Weapons/cannon_bullet.png");


	worldFonts.storeResource(Fonts_ID::ArialNarrow, "Resources/Fonts/arial_narrow.ttf");
}

void World::createWorld()
{
	enum class WorldObjects
	{
		Worm,
		StaticPaperBlock,
		DynamicPaperBlock,
	};

	std::ifstream worldMap("Resources/Maps/main_world.txt");
	std::string line;
	while (std::getline(worldMap, line))
	{
		// This is made for comments in the world file
		if (line[0] == '#' || line.empty())
			continue;

		std::stringstream ss(line);
		unsigned objectId;
		ss >> objectId;

		switch (objectId)
		{
		case static_cast<unsigned>(WorldObjects::Worm):
			{
				float positionX, positionY;
				ss >> positionX >> positionY;
				std::unique_ptr<Worm> worm = std::make_unique<Worm>(b2_World, worldTextures, worldFonts, worldWindow,
				                                                    sf::Vector2f(positionX, positionY),
				                                                    wormQueue);
				rootScene.pinNode(std::move(worm));
			}
			break;

		case static_cast<unsigned>(WorldObjects::StaticPaperBlock):
			{
				float positionX, positionY, width, height, rotation;
				ss >> positionX >> positionY >> width >> height >> rotation;
				std::unique_ptr<NodePhysicalSprite> staticPaperBlock = std::make_unique<NodePhysicalSprite>(
					b2_World, NodePhysicalBody::Physical_Types::Static_Type,
					worldTextures.getResourceReference(Textures_ID::Paper), sf::Vector2f(positionX, positionY),
					sf::Vector2f{width, height});
				if (rotation)
					staticPaperBlock->setRotation(rotation);
				rootScene.pinNode(std::move(staticPaperBlock));
			}
			break;

		case static_cast<unsigned>(WorldObjects::DynamicPaperBlock):
			{
				float positionX, positionY, width, height, rotation;
				ss >> positionX >> positionY >> width >> height >> rotation;
				std::unique_ptr<NodePhysicalSprite> dynamicPaperBlock = std::make_unique<NodePhysicalSprite>(
					b2_World, NodePhysicalBody::Physical_Types::Dynamic_Type,
					worldTextures.getResourceReference(Textures_ID::Paper), sf::Vector2f(positionX, positionY),
					sf::Vector2f{width, height});
				if (rotation)
					dynamicPaperBlock->setRotation(rotation);
				rootScene.pinNode(std::move(dynamicPaperBlock));
			}
			break;
		}
	}
}

void World::checkTurnTime()
{
	// On default there is no HideState
	static bool hideState = false;
	if (!wormQueue.empty())
	{
		sf::Time timeElapsed = roundClock.getElapsedTime();

		if (!hideState && ((timeElapsed > timePerTurn) || (wormQueue.front()->getCurrentState() ==
			State_ID::WormHideState)))
		{
			// Redundancy
			hideState = true;
			if (!(wormQueue.front()->getCurrentState() == State_ID::WormHideState))
				wormQueue.front()->activateState(State_ID::WormHideState);

			roundTimeText.setFillColor(sf::Color::Red);
			roundClock.restart();
			timeElapsed = sf::Time::Zero;
		}

		if (hideState && (timeElapsed > timePerHide))
		{
			Worm* worm = std::move(wormQueue.front());
			worm->activateState(State_ID::WormWaitState);
			wormQueue.pop_front();
			wormQueue.push_back(std::move(worm));
			wormQueue.front()->activateState(State_ID::WormPlayState);

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
		worldView.move(
			worldWindow.mapPixelToCoords(oldMouseCoordinates) - worldWindow.mapPixelToCoords(newMouseCoordinates));
		worldWindow.setView(worldView);

		// Fixes the posiiton of the timer
		roundTimeText.setPosition(
			worldWindow.mapPixelToCoords(sf::Vector2i(worldWindow.getSize().x / 2,
			                                          roundTimeText.getCharacterSize())));

		// Fixes the background
		backgroundSprite.setPosition(worldWindow.mapPixelToCoords({0, 0}));
	}
	oldMouseCoordinates = sf::Mouse::getPosition();
}
