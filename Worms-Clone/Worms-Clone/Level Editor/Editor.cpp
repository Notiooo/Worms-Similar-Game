#include "Editor.h"
#include <fstream>
#include <sstream>

Editor::Editor(sf::RenderWindow& window, const FontManager& fonts):
	editorWindow(window),
	editorView(window.getDefaultView()),
	fonts(fonts)
{
	loadResources();

	backgroundSprite.setTexture(textures.getResourceReference(Textures_ID::WorldBackground));
	backgroundSprite.setTextureRect(sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
	loadWorld();
}

void Editor::update(sf::Time deltaTime)
{
	for (auto& object : createdObjects)
		object.update(deltaTime);

	updateMouse();
	moveScreenWithMouse();
}

void Editor::handleEvent(const sf::Event& event)
{
	for (auto& object : createdObjects)
		object.handleEvent(event);
	
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
		editorView.move({ oldCoordinates - newCoordinates });
		editorWindow.setView(editorView);

		// Fixes the background
		backgroundSprite.setTextureRect(sf::IntRect(0, 0, editorView.getSize().x, editorView.getSize().y));
		backgroundSprite.setPosition(editorWindow.mapPixelToCoords({ 0, 0 }));
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
		backgroundSprite.setPosition(editorWindow.mapPixelToCoords({ 0, 0 }));
	}
	oldMouseCoordinates = sf::Mouse::getPosition();
}



void Editor::updateMouse()
{
	sf::Vector2f mousePosition = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

	for (auto& object : createdObjects)
		object.updateMouse(mousePosition);
}

void Editor::draw() const
{
	editorWindow.draw(backgroundSprite);

	for(const auto& object : createdObjects)
		editorWindow.draw(object);
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
	// Object that the world can create
	enum class WorldObjects
	{
		WormSpawnPoint,
		StaticPaperBlock,
		DynamicPaperBlock,
	};

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

		switch (objectId)
		{
			case static_cast<unsigned>(WorldObjects::WormSpawnPoint) :
			{
				float positionX, positionY;
				ss >> positionX >> positionY;

				newlyCreatedObject.setPosition(positionX, positionY);
				newlyCreatedObject.setName("Worm Spawn Point");
			}
			break;

			case static_cast<unsigned>(WorldObjects::StaticPaperBlock) :
			{
				float positionX, positionY, width, height, rotation;
				ss >> positionX >> positionY >> width >> height >> rotation;

				newlyCreatedObject.setPosition(positionX, positionY);
				newlyCreatedObject.setSize(width, height);
				newlyCreatedObject.setRotation(rotation);
				newlyCreatedObject.setName("Static Paper Block");
					
			}
			break;

			case static_cast<unsigned>(WorldObjects::DynamicPaperBlock) :
			{
				float positionX, positionY, width, height, rotation;
				ss >> positionX >> positionY >> width >> height >> rotation;

				newlyCreatedObject.setPosition(positionX, positionY);
				newlyCreatedObject.setSize(width, height);
				newlyCreatedObject.setRotation(rotation);
				newlyCreatedObject.setName("Dynamic Paper Block");
					
			}
			break;
		}
	}
}