#include "pch.h"
#include "GUI/FixedContainer.h"

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
	const auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
	
	for (auto& pinnedComponent : pinnedComponents)
		pinnedComponent->update(mousePosition);
	
	const auto leftUppercorner = window.mapPixelToCoords({ 0, 0 });
	setPosition(leftUppercorner);
	
	viewScale.x = window.getView().getSize().x / window.getDefaultView().getSize().x;
	viewScale.y = window.getView().getSize().y / window.getDefaultView().getSize().y;
	setScale(viewScale);
}

void GUI::FixedContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto& pinnedComponent : pinnedComponents)
		target.draw(*pinnedComponent, states);
}

