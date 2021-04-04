#ifndef EDITOR_H
#define EDITOR_H
#include <SFML/Graphics/RenderWindow.hpp>

#include "NodeEditorObject.h"
#include "../Nodes/NodeScene.h"
#include "../Resources/Resources.h"
#include <list>

#include "../GUI/Container.h"


/**
 * \brief An in-game editor that allows to edit the map
 */
class Editor
{
public:
	/**
	 * \brief Objects that the world can create
	 */
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
	
	/**
	 * \brief Saves the created world to a file
	 */
	void saveWorld();
	
	/**
	 * \brief Deletes objects mark as ready to be deleted
	 */
	void removeDestroyed();

private:
	/**
	 * \brief A function that allows to move around the editor with the middle mouse button
	 */
	void moveScreenWithMouse();

	/**
	 * \brief Function that loads the resources needed for the editor
	 */
	void loadResources();
	
	/**
	 * \brief A function that reads a map from a file and allows to edit it.
	 */
	void loadWorld();
	
	/**
	 * \brief Creates a map of identifiers to their text (std::string) equivalents (names).
	 */
	void loadInGameObjects();

	/**
	 * \brief Creates a menu for selecting an object to create
	 * \param mousePosition The place where it will create (appear) the menu.
	 */
	void createCreationMenu(const sf::Vector2f& mousePosition);

	
	/**
	 * \brief Creates an object with the given identifier on the given coordinates.
	 * \param objectId Identifier of the object to create
	 * \param mousePosition Coordinates in which the object should be created
	 */
	void createObject(unsigned objectId, const sf::Vector2f& mousePosition);

	// === Managers === //
	const FontManager& fonts; //!< Contains the necessary fonts for the editor
	TextureManager textures; //!< Contains the necessary textures for the editor

	// === Window and view === //
	sf::RenderWindow& editorWindow; //!< Window to which the editor is displayed
	sf::View editorView;
	float maxZoomFactor = 3.f; //!< Maximum zoom in and out inside the editor.
	sf::Sprite backgroundSprite; //!< Background of the editor

	// === In-game creator (menu) === //
	std::map<unsigned, std::string> inGameObjects; //!< Map object identifiers to their names (text representations).
	GUI::Container creationMenu; //!< Menu containing buttons that create an object specified for them
	sf::Vector2f mousePosition; //!< Current position of the mouse in relation to the game world
	std::list<NodeEditorObject> createdObjects; //!< A list containing all objects in the game world


};



#endif