#ifndef FIXEDCONTAINER_H
#define FIXEDCONTAINER_H
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>


#include "Container.h"

namespace GUI
{
	
	/**
	 * \brief A container for storing all kinds of interactive elements such as button
	 */
	class FixedContainer : public Container
	{
	public:
		FixedContainer(sf::RenderWindow& window);
		void store(std::unique_ptr<Component> component) override;

		void update();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::Vector2f getWindowScale();

	private:
		sf::Vector2f windowScale;
	};
	
}


#endif