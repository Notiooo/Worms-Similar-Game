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
		activateFunction();
}

void GUI::Button::setSize(int x, int y)
{
	sprite.setTextureRect(sf::IntRect(0, 0, x, y));
	centerOrigin(sprite);
}

GUI::Button::Button(const TextureManager& textures, const FontManager& fonts, sf::RenderWindow& window):
normalTexture(textures.getResourceReference(Textures_ID::Paper)),
selectedTexture(textures.getResourceReference(Textures_ID::Paper)),
text("", fonts.getResourceReference(Fonts_ID::ArialNarrow), 20),
window(window)
{
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.f);
	
	sprite.setTexture(normalTexture);
	centerOrigin(sprite);
}

void GUI::Button::setActiveFunction(std::function<void()> onActivate)
{
	activateFunction = std::move(onActivate);
}

void GUI::Button::setText(const std::string& _text)
{
	text.setString(_text);
	centerOrigin(text);
}

sf::FloatRect GUI::Button::getGlobalBounds()
{
	return sf::FloatRect(getPosition().x - sprite.getOrigin().x, getPosition().y - sprite.getOrigin().y,
		sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
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
		activate();
}

void GUI::Button::update()
{
	// If mouse has left the button
	if (isSelected() && !getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
		onLeave();

	// If mouse has entered the button
	if(!isSelected() && getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
		onEnter();
}

void GUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
	target.draw(text, states);
}