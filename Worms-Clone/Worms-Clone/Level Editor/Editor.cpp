#include "Editor.h"
#include <fstream>
#include <sstream>
#include <iostream>


#include "../WorldObjects.h"
#include "../GUI/Button.h"

Editor::Editor(sf::RenderWindow& window, const FontManager& fonts):
	fonts(fonts),
	editorWindow(window),
	editorView(window.getDefaultView()),
	creationMenu(window)
{
	loadResources();
	registerInGameObjects();

	backgroundSprite.setTexture(textures.getResourceReference(Textures_ID::WorldBackground));
	backgroundSprite.setTextureRect(sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
	loadWorld();
}

void Editor::update(sf::Time deltaTime)
{
	for (auto& object : createdObjects)
		object.update(deltaTime);

	creationMenu.update();

	updateMouse();
	moveScreenWithMouse();
}

void Editor::handleEvent(const sf::Event& event)
{
	for (auto& object : createdObjects)
		object.handleEvent(event);

	creationMenu.handleEvents(event);

	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.key.code == sf::Mouse::Right && creationMenu.isEmpty())
			createCreationMenu(mousePosition);

		if (event.key.code == sf::Mouse::Left)
			creationMenu.requestClear();
	}


	// Zoom in & zoom out the view
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		// Saves coordinates of the mouse on the screen
		const sf::Vector2f oldCoordinates{
			editorWindow.mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y})
		};

		// Zoom in & out, and also scales the timer size
		auto current_zoom = editorView.getSize().x;
		auto maximum_zoom = editorWindow.getDefaultView().getSize().x * maxZoomFactor;
		auto minimum_zoom = editorWindow.getDefaultView().getSize().x / maxZoomFactor;

		// Zooming in
		if (event.mouseWheelScroll.delta > 0)
		{
			if (current_zoom > minimum_zoom)
				editorView.zoom(1.f / 1.1f);
		}
			// Zooming out
		else
		{
			if (current_zoom < maximum_zoom)
				editorView.zoom(1.1f);
		}

		// Sets the new view to the window
		editorWindow.setView(editorView);

		// Reads the "new" position of the mouse (it changed since the zoom is different)
		const sf::Vector2f newCoordinates{
			editorWindow.mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y})
		};

		// It moves the view, so it will "zoom into" the cursor, and not into the center of the screen
		editorView.move({oldCoordinates - newCoordinates});
		editorWindow.setView(editorView);

		// Fixes the background
		backgroundSprite.setTextureRect(sf::IntRect(0, 0, editorView.getSize().x, editorView.getSize().y));
		backgroundSprite.setPosition(editorWindow.mapPixelToCoords({0, 0}));
	}
}

void Editor::moveScreenWithMouse()
{
	static sf::Vector2i oldMouseCoordinates = sf::Mouse::getPosition();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		sf::Vector2i newMouseCoordinates = sf::Mouse::getPosition();
		editorView.move(
			editorWindow.mapPixelToCoords(oldMouseCoordinates) - editorWindow.mapPixelToCoords(newMouseCoordinates));
		editorWindow.setView(editorView);

		// Fixes the background
		backgroundSprite.setPosition(editorWindow.mapPixelToCoords({0, 0}));
	}
	oldMouseCoordinates = sf::Mouse::getPosition();
}


void Editor::updateMouse()
{
	mousePosition = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

	for (auto& object : createdObjects)
		object.updateMouse(mousePosition);
}

void Editor::draw() const
{
	editorWindow.draw(backgroundSprite);

	for (const auto& object : createdObjects)
		editorWindow.draw(object);

	editorWindow.draw(creationMenu);
}

void Editor::loadResources()
{
	textures.storeResource(Textures_ID::Paper, "Resources/Textures/World/paper_texture.png");
	textures.storeResource(Textures_ID::WorldBackground, "Resources/Textures/World/background_texture.png");
	textures.getResourceReference(Textures_ID::Paper).setRepeated(true);
	textures.getResourceReference(Textures_ID::WorldBackground).setRepeated(true);
}


void Editor::loadWorld()
{
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

		createdObjects.emplace_back(textures, fonts);

		NodeEditorObject& newlyCreatedObject = createdObjects.back();
		newlyCreatedObject.setId(objectId);

		switch (objectId)
		{
		case static_cast<unsigned>(WorldObjects::WormSpawnPoint):
			{
				float positionX, positionY;
				ss >> positionX >> positionY;

				newlyCreatedObject.setPosition(positionX, positionY);
				newlyCreatedObject.setName(inGameObjects[objectId]);
			}
			break;

		case static_cast<unsigned>(WorldObjects::StaticPaperBlock):
			{
				float positionX, positionY, width, height, rotation;
				ss >> positionX >> positionY >> width >> height >> rotation;

				newlyCreatedObject.setPosition(positionX, positionY);
				newlyCreatedObject.setSize(width, height);
				newlyCreatedObject.setRotation(rotation);
				newlyCreatedObject.setName(inGameObjects[objectId]);
			}
			break;

		case static_cast<unsigned>(WorldObjects::DynamicPaperBlock):
			{
				float positionX, positionY, width, height, rotation;
				ss >> positionX >> positionY >> width >> height >> rotation;

				newlyCreatedObject.setPosition(positionX, positionY);
				newlyCreatedObject.setSize(width, height);
				newlyCreatedObject.setRotation(rotation);
				newlyCreatedObject.setName(inGameObjects[objectId]);
			}
			break;

		case static_cast<unsigned>(WorldObjects::Water) :
		{
			float positionX, positionY, width, height, rotation;
			ss >> positionX >> positionY >> width >> height >> rotation;

			newlyCreatedObject.setPosition(positionX, positionY);
			newlyCreatedObject.setSize(width, height);
			newlyCreatedObject.setRotation(rotation);
			newlyCreatedObject.setName(inGameObjects[objectId]);
		}
		break;
		case static_cast<unsigned>(WorldObjects::DestructibleBlock) :
		{
			float positionX, positionY, width, height, rotation;
			ss >> positionX >> positionY >> width >> height >> rotation;

			newlyCreatedObject.setPosition(positionX, positionY);
			newlyCreatedObject.setSize(width, height);
			newlyCreatedObject.setRotation(rotation);
			newlyCreatedObject.setName(inGameObjects[objectId]);
		}
		break;
		}
	}
	worldMap.close();
}

void Editor::registerInGameObjects()
{
	inGameObjects.insert({static_cast<unsigned>(WorldObjects::WormSpawnPoint), "Worm Spawn Point"});
	inGameObjects.insert({static_cast<unsigned>(WorldObjects::StaticPaperBlock), "Static Paper Block"});
	inGameObjects.insert({static_cast<unsigned>(WorldObjects::DynamicPaperBlock), "Dynamic Paper Block" });
	inGameObjects.insert({ static_cast<unsigned>(WorldObjects::DestructibleBlock), "Destructible Block" });
	inGameObjects.insert({static_cast<unsigned>(WorldObjects::Water), "Water"});
}

void Editor::createCreationMenu(const sf::Vector2f& mousePosition)
{
	// For every registered in-game object
	for (auto& object : inGameObjects)
	{
		// Creates a button
		auto button = std::make_unique<GUI::Button>(textures, fonts);
		button->setText(object.second);
		button->matchSizeToText(10.f);

		// The first one should be placed at posiiton of cursor
		// And the other one should be placed below the predecessor
		if (creationMenu.isEmpty())
			button->setPosition(mousePosition);
		else
			button->setPositionBelow(creationMenu.back(), 20.f);

		// Activation of the button leads to creating object
		// and "closing" the menu
		button->setActiveFunction([this, mousePosition, object](GUI::Button& button)
		{
			createObject(object.first, mousePosition);
			creationMenu.requestClear();
		});

		creationMenu.store(std::move(button));
	}
}

void Editor::createObject(unsigned objectId, const sf::Vector2f& position)
{
	createdObjects.emplace_back(textures, fonts);

	NodeEditorObject& newlyCreatedObject = createdObjects.back();
	newlyCreatedObject.setId(objectId);
	newlyCreatedObject.setPosition(mousePosition.x, mousePosition.y);
	newlyCreatedObject.setName(inGameObjects[objectId]);
}


template <typename ...Args>
void printLine(std::ofstream& os, Args&&... args)
{
	((os << std::forward<Args>(args) << ' '), ...) << '\n';
}

void Editor::saveWorld()
{
	std::ofstream worldMap("Resources/Maps/main_world.txt");

	for (auto& object : createdObjects)
	{
		switch (object.getId())
		{
		case static_cast<unsigned>(WorldObjects::WormSpawnPoint):
			printLine(worldMap, object.getId(), object.getPosition().x, object.getPosition().y);
			break;

		case static_cast<unsigned>(WorldObjects::StaticPaperBlock):
			printLine(worldMap, object.getId(), object.getPosition().x, object.getPosition().y,
			          object.getSize().x, object.getSize().y, object.getRotation());
			break;

		case static_cast<unsigned>(WorldObjects::DynamicPaperBlock):
			printLine(worldMap, object.getId(), object.getPosition().x, object.getPosition().y,
			          object.getSize().x, object.getSize().y, object.getRotation());
			break;

		case static_cast<unsigned>(WorldObjects::Water) :
			printLine(worldMap, object.getId(), object.getPosition().x, object.getPosition().y,
				object.getSize().x, object.getSize().y, object.getRotation());
			break;

		case static_cast<unsigned>(WorldObjects::DestructibleBlock) :
			printLine(worldMap, object.getId(), object.getPosition().x, object.getPosition().y,
				object.getSize().x, object.getSize().y, object.getRotation());
			break;
		}
	}
	worldMap.close();
}

void Editor::removeDestroyed()
{
	auto removal_mark = std::remove_if(createdObjects.begin(), createdObjects.end(),
	                                   std::mem_fn(&NodeEditorObject::isDestroyed));
	createdObjects.erase(removal_mark, createdObjects.end());
}
