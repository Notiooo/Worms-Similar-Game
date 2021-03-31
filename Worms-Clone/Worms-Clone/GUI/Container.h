#ifndef CONTAINER_H
#define CONTAINER_H
#include <vector>

#include "Component.h"

namespace GUI
{
	
	class Container : Component
	{
	public:

		void store(std::unique_ptr<Component> component);

		void handleEvents(const sf::Event& event) override;

		void update() override;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		std::vector<std::unique_ptr<Component>> pinnedComponents;
	};
	
}


#endif