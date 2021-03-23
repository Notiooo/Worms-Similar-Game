#include "NodeText.h"
#include "SFML/Graphics/RenderTarget.hpp"

NodeText::NodeText(const FontManager& fonts, unsigned int size=12, bool outline=true):
	nodeText()
{
	nodeText.setFont(fonts.getResourceReference(Fonts_ID::Arial_Narrow));
	nodeText.setCharacterSize(size);
	nodeText.setFillColor(sf::Color::White);
	if (outline)
	{
		nodeText.setOutlineColor(sf::Color::Black);
		nodeText.setOutlineThickness(1.f);
	}
}

NodeText::NodeText(const sf::Font& font, const std::string& text = "", unsigned int size = 12, bool outline = true)
{
	nodeText.setFont(font);
	nodeText.setCharacterSize(size);
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
