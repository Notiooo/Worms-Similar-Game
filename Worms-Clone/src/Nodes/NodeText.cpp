#include "pch.h"
#include "NodeText.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Utils/utils.h"

NodeText::NodeText(const sf::Font& font, unsigned int size=12, bool outline=true):
	NodeText(font, "", size, outline)
{

}

NodeText::NodeText(const sf::Font& font, const std::string& text = "", unsigned int size = 12, bool outline = true):
	nodeText(text, font, size)
{
	nodeText.setFillColor(sf::Color::White);
	if (outline)
	{
		nodeText.setOutlineColor(sf::Color::Black);
		nodeText.setOutlineThickness(1.f);
	}
}

void NodeText::setString(const std::string& text)
{
	nodeText.setString(text);
	centerOrigin(nodeText);
}

void NodeText::setOutline(float thickness, sf::Color color)
{
	nodeText.setOutlineColor(color);
	nodeText.setOutlineThickness(thickness);
}

void NodeText::setColor(sf::Color color)
{
	nodeText.setFillColor(color);
}

void NodeText::setSize(unsigned int size)
{
	nodeText.setCharacterSize(size);
}

void NodeText::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nodeText, states);
}
