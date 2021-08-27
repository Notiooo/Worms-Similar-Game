#ifndef NODEEDITOROBJECT_H
#define NODEEDITOROBJECT_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "Resources/Resources.h"

/**
 * \brief Editor object that allows to rotate and position on the map.
 * Contains information needed for the map export to file
 */
class NodeEditorObject final : public sf::Drawable, public sf::Transformable
{
public:
	NodeEditorObject(const TextureManager& textures, const FontManager& fonts);

	/**
	 * \brief Updates object states related to mouse position
	 * \param mousePosition Current mouse coordinates inside the game world
	 */
	void updateMouse(const sf::Vector2f& mousePosition);

	/**
	 * \brief Updates the status/logic of the node
	 * \param deltaTime the time that has passed since the last frame.
	 */
	void update(sf::Time deltaTime);

	/**
	 * \brief Draws this node to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	void handleEvent(const sf::Event& event);

	/**
	 * \brief Sets the object's name (textual representation).
	 * \param name New name of the object
	 */
	void setName(const std::string& name);
	
	/**
	 * \brief Sets the new object size
	 * \param width New object width
	 * \param height New object height
	 */
	void setSize(float width, float height);
	
	/**
	 * \brief Sets the object the given identifier (which the object will now represent).
	 * \param id Identifier of object to represent.
	 */
	void setId(unsigned id) noexcept;

	/**
	 * \brief Get the global bounding rectangle of the entity
	 * \return GLobal bounding rectangle of the entity
	 */
	sf::FloatRect getGlobalBounds() const;
	
	/**
	 * \brief Returns the dimensions of this object
	 * \return Dimensions of this object
	 */
	sf::Vector2f getSize() const;
	
	/**
	 * \brief Returns the object identifier that this object represents.
	 * \return The object identifier that this object represents
	 */
	unsigned getId() const noexcept;

	/**
	 * \brief A function that checks if the mouse hovers over an object.
	 * \return True if user hovers with the mouse over an object, false otherwise
	 */
	bool isSelected() const noexcept;

	/**
	 * \brief A function that executes once when the user hovers over an object.
	 */
	void select() noexcept;
	
	/**
	 * \brief The function that executes once when the user leaves the object with the cursor.
	 */
	void unselect() noexcept;
	
	/**
	 * \brief Checks if objects are activated (user selected / pressed).
	 * \return True if it is activated, false otherwise
	 */
	bool isActivated() const noexcept;
	
	/**
	 * \brief The function that is executed when user click on the object (check it) -- otherwise known as deactivating the object.
	 */
	void activate();
	
	/**
	 * \brief The function that is executed when "unchecking" -- otherwise known as deactivating the object.
	 */
	void deactivate();

	/**
	 * \brief A function that marks an object as ready for deletion.
	 */
	void setDestroyed() noexcept;
	/**
	 * \brief A function that checks if the object is ready to be deleted.
	 * \return True if ready for removal, false otherwise
	 */
	bool isDestroyed() const noexcept;

private:
	// === Button Mechanics === //
	sf::Sprite nodeSprite; //!< Graphic visualization of the object
	sf::Text nodeName; //!< Name of the object (displayed on the editor screen).
	sf::Vector2f size;	//!< Size of the object
						//!< Must be tracked because TextureRect stores values
						//!< in ints which can lead to no value being added when
						//!< multiplied by deltaTime (they're smaller than zero).
	
	bool _isSelected = false; //!< Flag indicating whether the user hovered over the object
	bool _isActivated = false; //!< A flag indicating whether the user has hovered over and clicked (activated) the object.

	// === Speed of object editing === //
	float movingSpeed = 100.f; //!< The speed at which objects can be moved inside the editor.
	float rotationSpeed = 50.f; //!< The speed at which objects inside the editor are rotated.
	float sizeChangeSpeed = 150.f; //!< The speed at which objects inside the editor are scaled.

	// === Object properties === //
	unsigned objectId; //!< An object identifier that defines what the object will be inside the game.
	bool _isDestroyed = false; //!< Specifies whether the object is marked as ready for deletion.

};

#endif