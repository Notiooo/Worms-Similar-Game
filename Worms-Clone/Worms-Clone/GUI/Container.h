#ifndef CONTAINER_H
#define CONTAINER_H
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>


#include "Component.h"

namespace GUI
{
	/**
	 * \brief A container for storing all kinds of interactive elements such as button.
	 * The container is inside the game world and does not follow the screen.
	 */
	class Container : public sf::Transformable, public sf::Drawable
	{
	public:

		Container(const sf::RenderWindow& window);
		
		/**
		 * \brief Adds a component to the container
		 * \param component Component to add
		 */
		virtual void store(std::unique_ptr<Component> component);

		/**
		 * \brief Checks and returns information whether a container is empty
		 * \return True if container is empty, false otherwise
		 */
		bool isEmpty();

		/**
		 * \brief Returns the reference to the element at the front of the container
		 * \return Reference to the element at the front of the container
		 */
		Component& front();

		/**
		 * \brief Returns the reference to the element at the back of the container
		 * \return Reference to the element at the back of the container
		 */
		Component& back();

		/**
		 * \brief It takes input (event) from the user and interprets it,
		 * and then passes it to all components inside the container
		 * \param event user input
		 */
		void handleEvents(const sf::Event& event);

		/**
		 * \brief Updates the logic/status of all components inside the container
		 */
		void update();

		/**
		 * \brief Draws all components inside the container to the passed target
		 * \param target where it should be drawn to
		 * \param states provides information about rendering process (transform, shader, blend mode)
		 */
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/**
		 * \brief Request for clearing the container -- the request
		 * is only processed at the beginning of the next iteration
		 */
		void requestClear();

		// Inserts into the public scope the necessary functions that are normally hidden
		using sf::Transformable::setOrigin;
		using sf::Transformable::setPosition;
		using sf::Transformable::setRotation;

	protected:
		std::vector<std::unique_ptr<Component>> pinnedComponents; //!< All components that have been pinned to this container
		const sf::RenderWindow& window; //!< Window into which the content of the container is displayed

		bool clearRequest = false; //!< A flag indicating whether there is a request to clear the container
	};
	
}


#endif