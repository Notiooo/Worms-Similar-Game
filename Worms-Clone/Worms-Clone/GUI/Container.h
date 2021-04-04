#ifndef CONTAINER_H
#define CONTAINER_H
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>


#include "Component.h"

namespace GUI
{
	
	/**
	 * \brief A container for storing all kinds of interactive elements such as button
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

		bool isEmpty();
		Component& front();
		Component& back();

		void handleEvents(const sf::Event& event);
		void update();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void requestClear();

		using sf::Transformable::setOrigin;
		using sf::Transformable::setPosition;
		using sf::Transformable::setRotation;

	protected:
		std::vector<std::unique_ptr<Component>> pinnedComponents;
		const sf::RenderWindow& window;

		bool clearRequest = false;
	};
	
}


#endif