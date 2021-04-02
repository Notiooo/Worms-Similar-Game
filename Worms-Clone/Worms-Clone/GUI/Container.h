#ifndef CONTAINER_H
#define CONTAINER_H
#include <vector>

#include "Component.h"

namespace GUI
{
	
	/**
	 * \brief A container for storing all kinds of interactive elements such as button
	 */
	class Container : Component
	{
	public:

		/**
		 * \brief Adds a component to the container
		 * \param component Component to add
		 */
		void store(std::unique_ptr<Component> component);

		void handleEvents(const sf::Event& event) override;
		void update() override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		using sf::Transformable::setOrigin;

	private:
		std::vector<std::unique_ptr<Component>> pinnedComponents;
	};
	
}


#endif