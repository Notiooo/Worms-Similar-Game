#include "Button.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "../utils.h"

void GUI::Button::onEnter()
{
	GUI::Component::onEnter();

	sprite.setTexture(selectedTexture);
	centerOrigin(sprite);
	sprite.setScale(1.2, 1.2);

	centerOrigin(text);
	text.setScale(1.2, 1.2);
}

void GUI::Button::onLeave()
{
	GUI::Component::onLeave();

	sprite.setTexture(normalTexture);
	centerOrigin(sprite);
	sprite.setScale(1, 1);

	centerOrigin(text);
	text.setScale(1, 1);
}

void GUI::Button::activate()
{
	GUI::Component::activate();

	if (activateFunction)
		activateFunction(*this);
}

void GUI::Button::deactivate()
{
	GUI::Component::deactivate();

	if (deactivateFunction)
		deactivateFunction(*this);
}

void GUI::Button::setSize(int width, int height)
{
	sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	centerOrigin(sprite);
}

GUI::Button::Button(const TextureManager& textures, const FontManager& fonts):
normalTexture(textures.getResourceReference(Textures_ID::Paper)),
selectedTexture(textures.getResourceReference(Textures_ID::Paper)),
text("", fonts.getResourceReference(Fonts_ID::ArialNarrow), 20)
{
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color(47, 60, 76, 255));
	text.setOutlineThickness(1.5f);
	
	sprite.setTexture(normalTexture);
	centerOrigin(sprite);
}

void GUI::Button::setActiveFunction(std::function<void(Button&)> onActivate)
{
	activateFunction = std::move(onActivate);
}

void GUI::Button::setDeactiveFunction(std::function<void(Button&)> onDeactivate)
{
	deactivateFunction = std::move(onDeactivate);
}

void GUI::Button::setText(const std::string& _text)
{
	text.setString(_text);
	centerOrigin(text);
}

sf::FloatRect GUI::Button::getGlobalBounds() const
{
	const auto rectLeft = getPosition().x - sprite.getOrigin().x * getScale().x;
	const auto rectTop = getPosition().y - sprite.getOrigin().y * getScale().y;
	const auto rectWidth = sprite.getLocalBounds().width * sprite.getScale().x * getScale().x;
	const auto rectHeight = sprite.getLocalBounds().height * sprite.getScale().y * getScale().y;
	
	return sf::FloatRect(rectLeft, rectTop, rectWidth, rectHeight);
}

sf::FloatRect GUI::Button::getLocalBounds() const
{
	return sprite.getLocalBounds();
}

void GUI::Button::matchSizeToText(float padding)
{
	sprite.setTextureRect(sf::IntRect(0, 0, 
		text.getLocalBounds().width * text.getScale().x + padding, 
		text.getLocalBounds().height * text.getScale().y + padding));
	
	centerOrigin(sprite);
}

void GUI::Button::handleEvents(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && isSelected())
	{
		if(event.key.code == sf::Mouse::Left)
			activate();
		if (event.key.code == sf::Mouse::Right)
			deactivate();
	}
}

void GUI::Button::update(sf::Vector2f mousePosition)
{
	// If mouse has left the button
	if (isSelected() && !getGlobalBounds().contains(mousePosition))
		onLeave();

	// If mouse has entered the button
	if (!isSelected() && getGlobalBounds().contains(mousePosition))
		onEnter();
}

void GUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
	target.draw(text, states);
}