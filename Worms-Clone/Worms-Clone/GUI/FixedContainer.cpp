#include "FixedContainer.h"

#include <SFML/Graphics/RenderTarget.hpp>

GUI::FixedContainer::FixedContainer(sf::RenderWindow& window):
Container(window)
{
	
}

void GUI::FixedContainer::store(std::unique_ptr<Component> component)
{
	pinnedComponents.push_back(std::move(component));
}

void GUI::FixedContainer::update()
{
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

	for (auto& pinnedComponent : pinnedComponents)
		pinnedComponent->update(mousePosition);
	
	sf::Vector2f leftUppercorner = window.mapPixelToCoords({ 0, 0 });
	setPosition(leftUppercorner);

	windowScale.x = window.getView().getSize().x / window.getDefaultView().getSize().x;
	windowScale.y = window.getView().getSize().y / window.getDefaultView().getSize().y;
	setScale(windowScale);
}

void GUI::FixedContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto& pinnedComponent : pinnedComponents)
		target.draw(*pinnedComponent, states);
}

sf::Vector2f GUI::FixedContainer::getWindowScale()
{
	return windowScale;
}

