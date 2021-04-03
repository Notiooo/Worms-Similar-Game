#ifndef EDITOR_H
#define EDITOR_H
#include <SFML/Graphics/RenderWindow.hpp>

#include "NodeEditorObject.h"
#include "../Nodes/NodeScene.h"
#include "../Resources/Resources.h"
#include <list>


class Editor
{
public:
	Editor(sf::RenderWindow& window, const FontManager& fonts);

	void update(sf::Time deltaTime);
	void updateMouse();

	void handleEvent(const sf::Event& event);
	void draw() const;

private:
	void moveScreenWithMouse();
	void loadResources();
	void loadWorld();
	
	sf::RenderWindow& editorWindow;
	sf::View editorView;
	float maxZoomFactor = 3.f;
	
	const FontManager& fonts;
	TextureManager textures;
	sf::Sprite backgroundSprite;
	std::list<NodeEditorObject> createdObjects;
};



#endif