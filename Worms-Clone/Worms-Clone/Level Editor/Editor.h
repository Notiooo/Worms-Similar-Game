#ifndef EDITOR_H
#define EDITOR_H
#include <SFML/Graphics/RenderWindow.hpp>

#include "NodeEditorObject.h"
#include "../Nodes/NodeScene.h"
#include "../Resources/Resources.h"
#include <list>

#include "../GUI/Container.h"


class Editor
{
public:
	// Object that the world can create
	enum class WorldObjects
	{
		WormSpawnPoint,
		StaticPaperBlock,
		DynamicPaperBlock,
		Counter,
	};

	Editor(sf::RenderWindow& window, const FontManager& fonts);

	void update(sf::Time deltaTime);
	void updateMouse();

	void handleEvent(const sf::Event& event);
	void draw() const;
	void saveWorld();
	void removeDestroyed();

private:
	void moveScreenWithMouse();
	void loadResources();
	void loadWorld();
	void loadInGameObjects();
	void createCreationMenu(const sf::Vector2f& mousePosition);
	void createObject(unsigned objectId, const sf::Vector2f& mousePosition);
	
	sf::RenderWindow& editorWindow;
	sf::View editorView;
	float maxZoomFactor = 3.f;
	
	const FontManager& fonts;
	TextureManager textures;
	sf::Sprite backgroundSprite;
	std::list<NodeEditorObject> createdObjects;
	sf::Vector2f mousePosition;

	std::map<unsigned, std::string> inGameObjects;

	GUI::Container creationMenu;
};



#endif