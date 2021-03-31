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
