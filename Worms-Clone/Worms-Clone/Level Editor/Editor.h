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
	Editor(sf::RenderWindow& window, const FontManager& fonts);

	/**
	 * \brief Updates the status/logic of the editor
	 * \param deltaTime the time that has passed since the last frame.
	 */
	void update(sf::Time deltaTime);

	/**
	 * \brief Updates editor logic and status related to the mouse
	 */
	void updateMouse();

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	void handleEvent(const sf::Event& event);

	
	/**
	 * \brief Draws the editor and all its contents into the application window
	 */
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
	void registerInGameObjects();

	/**
	 * \brief Creates a menu for selecting an object to create
	 * \param mousePosition The place where it will create (appear) the menu.
	 */
	void createCreationMenu(const sf::Vector2f& mousePosition);

	
	/**
	 * \brief Creates an object with the given identifier on the given coordinates.
	 * \param objectId Identifier of the object to create
	 * \param position Coordinates in which the object should be created
	 */
	void createObject(unsigned objectId, const sf::Vector2f& position);

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