#include "Container.h"

#include <SFML/Graphics/RenderTarget.hpp>

GUI::Container::Container(const sf::RenderWindow& window):
window(window)
{
}

void GUI::Container::store(std::unique_ptr<Component> component)
{
	pinnedComponents.push_back(std::move(component));
}

bool GUI::Container::isEmpty()
{
	return pinnedComponents.empty();
}

GUI::Component& GUI::Container::front()
{
	return *pinnedComponents.front();
}

GUI::Component& GUI::Container::back()
{
	return *pinnedComponents.back();
}


void GUI::Container::handleEvents(const sf::Event& event)
{
	if(clearRequest)
	{
		pinnedComponents.clear();
		clearRequest = false;
	}
	
	for (auto& pinnedComponent : pinnedComponents)
		pinnedComponent->handleEvents(event);
}

void GUI::Container::update()
{
	sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	
	for (auto& pinnedComponent : pinnedComponents)
		pinnedComponent->update(mousePosition);
}

void GUI::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto& pinnedComponent : pinnedComponents)
		target.draw(*pinnedComponent, states);
}

void GUI::Container::requestClear()
{
	clearRequest = true;
}
