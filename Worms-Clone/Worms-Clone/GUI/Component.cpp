#include "Component.h"


GUI::Component::Component()
{
}

GUI::Component::~Component()
{
}

bool GUI::Component::isSelected() const
{
	return _isSelected;
}

void GUI::Component::onEnter()
{
	_isSelected = true;
}

void GUI::Component::onLeave()
{
	_isSelected = false;
}

bool GUI::Component::isActive() const
{
	return _isActive;
}

void GUI::Component::activate()
{
	_isActive = true;
}

void GUI::Component::deactivate()
{
	_isActive = false;
}

void GUI::Component::setPositionBelow(const Component& object, float padding)
{
	sf::Vector2f pos(object.getPosition().x, object.getPosition().y);

	sf::Vector2f offset = sf::Vector2f(0, object.getGlobalBounds().height);

	padding *= getScale().y;
	offset *= getScale().y;
	setPosition(pos + offset + sf::Vector2f(0, padding));
}
