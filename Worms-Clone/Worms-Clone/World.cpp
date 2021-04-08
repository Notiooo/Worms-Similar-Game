#include "World.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Nodes/Physical/Specified/Worm/Worm.h"
#include <memory>
#include "Nodes/Physical/Specified/Worm/Weapons/Bullet.h"
#include <fstream>
#include <sstream>
#include <random>

#include "WorldObjects.h"
#include "Nodes/NodeWater.h"


World::World(sf::RenderWindow& window, int _wormAmount, int _numberOfTeams) :
	worldWindow(window),
	worldView(window.getDefaultView()),
	b2_World(b2Vec2(0.f, 9.8f)),
	debugDraw(window),
	wormAmount(_wormAmount),
	essentials({ &b2_World, &worldWindow, &worldTextures, &worldFonts }),
	numberOfTeams(_numberOfTeams)
{
	// Tells the physical world what to draw
	b2_World.SetDebugDraw(&debugDraw);
	
	loadResources();
	createWorld();

	backgroundSprite.setTexture(worldTextures.getResourceReference(Textures_ID::WorldBackground));
	backgroundSprite.setTextureRect(sf::IntRect(0, 0, worldView.getSize().x, worldView.getSize().y));

	
	b2_World.SetContactListener(&worldListener);
}

void World::update(sf::Time deltaTime)
{
	moveScreenWithMouse();
	
	// Update the Game World
	rootScene.update(deltaTime);

	// Check if there is anything to remove
	rootScene.removeDestroyed();

	// Update the physical world
	b2_World.Step(1 / 60.f, 8, 3);
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
				worldView.zoom(1.f / 1.1f);
		}
		// Zooming out
		else
		{
			if (current_zoom < maximum_zoom)
				worldView.zoom(1.1f);
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

bool World::isGameFinished() const
{
	return worldGameManager->isDestroyed();
}

void World::loadResources()
{

	// ==== Weapons ==== //

	// Bazooka
	worldTextures.storeResource(Textures_ID::Bazooka, "Resources/Textures/Weapons/bazooka.png");
	worldTextures.storeResource(Textures_ID::BazookaThumbnail, "Resources/Textures/Weapons/bazooka_thumbnail.png");
	worldTextures.storeResource(Textures_ID::BazookaBullet, "Resources/Textures/Weapons/bazooka_bullet.png");

	// Cannon
	worldTextures.storeResource(Textures_ID::Cannon, "Resources/Textures/Weapons/cannon.png");
	worldTextures.storeResource(Textures_ID::CannonThumbnail, "Resources/Textures/Weapons/cannon_thumbnail.png");
	worldTextures.storeResource(Textures_ID::CannonBullet, "Resources/Textures/Weapons/cannon_bullet.png");

	// Grenade
	worldTextures.storeResource(Textures_ID::Grenade, "Resources/Textures/Weapons/grenade.png");
	worldTextures.storeResource(Textures_ID::GrenadeThumbnail, "Resources/Textures/Weapons/grenade_thumbnail.png");
	worldTextures.storeResource(Textures_ID::GrenadeBullet, "Resources/Textures/Weapons/grenade_bullet.png");

	// === Worm === //
	worldTextures.storeResource(Textures_ID::AnExemplaryWorm, "Resources/Textures/An_example_worm.png");
	worldTextures.storeResource(Textures_ID::HitWorm, "Resources/Textures/Worm_Hit.png");
	worldTextures.storeResource(Textures_ID::DeadWorm, "Resources/Textures/Dead_Worm.png");
	worldTextures.storeResource(Textures_ID::Inventory, "Resources/Textures/Weapons/Inventory/background.png");


	// === World === //
	worldTextures.storeResource(Textures_ID::WorldBackground, "Resources/Textures/World/background_texture.png");
	worldTextures.getResourceReference(Textures_ID::WorldBackground).setRepeated(true);
	
	worldTextures.storeResource(Textures_ID::Paper, "Resources/Textures/World/paper_texture.png");
	worldTextures.getResourceReference(Textures_ID::Paper).setRepeated(true);
	
	worldTextures.storeResource(Textures_ID::Rope, "Resources/Textures/World/rope.png");
	worldTextures.getResourceReference(Textures_ID::Rope).setRepeated(true);

	worldTextures.storeResource(Textures_ID::Water, "Resources/Textures/World/water.png");
	worldTextures.getResourceReference(Textures_ID::Water).setRepeated(true);

	// === Fonts ===//
	worldFonts.storeResource(Fonts_ID::ArialNarrow, "Resources/Fonts/arial_narrow.ttf");
}

void World::createWorld()
{
	// Those layers helps to maintain order of drawing
	for(std::size_t i = 0; i < static_cast<unsigned>(WorldLayers::Counter); ++i)
	{
		NodeScene::Node newLayer(new NodeScene());
		worldLayers[i] = newLayer.get();
		rootScene.pinNode(std::move(newLayer));
	}

	// Setup the GameplayManager
	std::unique_ptr<GameplayManager> gameManager = std::make_unique<GameplayManager>(b2_World, worldTextures, worldFonts, worldWindow);
	worldGameManager = gameManager.get();
	worldLayers[static_cast<unsigned>(WorldLayers::Foreground)]->pinNode(std::move(gameManager));

	std::vector<sf::Vector2f> wormSpawnPoints;

	// Code that reads from the file particular commands and generates the world
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
		case static_cast<unsigned>(WorldObjects::WormSpawnPoint):
			{
				float positionX, positionY;
				ss >> positionX >> positionY;

				wormSpawnPoints.emplace_back(positionX, positionY);
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
				worldLayers[static_cast<unsigned>(WorldLayers::Middle)]->pinNode(std::move(staticPaperBlock));
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
				worldLayers[static_cast<unsigned>(WorldLayers::Middle)]->pinNode(std::move(dynamicPaperBlock));
			}
			break;

			case static_cast<unsigned>(WorldObjects::Water) :
			{
				float positionX, positionY, width, height, rotation;
				ss >> positionX >> positionY >> width >> height >> rotation;

				std::unique_ptr<NodeWater> water = std::make_unique<NodeWater>(b2_World, worldTextures.getResourceReference(Textures_ID::Water));
				water->setSize(width, height);
				water->setPosition(positionX, positionY);

				worldLayers[static_cast<unsigned>(WorldLayers::Background)]->pinNode(std::move(water));
			}
			break;
		}
	}

	// Spawn Worms

	std::random_device r;
	std::default_random_engine e(r());
	std::shuffle(wormSpawnPoints.begin(), wormSpawnPoints.end(), e);

	static std::array<std::string, 8> wormNames{ "Steve", "John", "Patrick", "Caroline", "Julia", "Richard", "David", "Robert" };
	static std::array<sf::Color, 5> wormColors{ sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Red };

	if (wormColors.size() < numberOfTeams)
		throw std::runtime_error("Not enough color teams!");
	
	auto wormNameIter = wormNames.cbegin();
	auto wormColorsIter = wormColors.cbegin();
	auto wormSpawnPointIter = wormSpawnPoints.cbegin();

	// For testing purposes they are hardcoded here
	for(int i = 0; i < numberOfTeams; ++i)
	{
		for (int j = 0; j < wormAmount; ++j)
		{
			if (wormNameIter == wormNames.cend())
				wormNameIter = wormNames.cbegin();

			if (wormSpawnPointIter == wormSpawnPoints.cend())
			{
				for (auto& spawnPoint : wormSpawnPoints)
					spawnPoint.y -= 100;
				wormSpawnPointIter = wormSpawnPoints.cbegin();
			}
			
			worldGameManager->addWorm(std::string(*wormNameIter++ + " The Worm"), *wormColorsIter, *wormSpawnPointIter++);
		}
		++wormColorsIter;
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

		// Fixes the background
		backgroundSprite.setPosition(worldWindow.mapPixelToCoords({0, 0}));
	}
	oldMouseCoordinates = sf::Mouse::getPosition();
}
