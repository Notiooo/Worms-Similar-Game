#ifndef FIXEDCONTAINER_H
#define FIXEDCONTAINER_H
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>


#include "Container.h"

namespace GUI
{
	
	/**
	 * \brief A container in which interface elements are permanently attached to the game window
	 */
	class FixedContainer : public Container
	{
	public:
		FixedContainer(sf::RenderWindow& window);

		/**
		 * \brief Adds a component to the container
		 * \param component Component to add
		 */
		void store(std::unique_ptr<Component> component) override;

		void update();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::Vector2f getWindowScale();

	private:
		sf::Vector2f windowScale;
	};
	
}


#endif