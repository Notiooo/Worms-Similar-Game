#include "NodeText.h"
#include "SFML/Graphics/RenderTarget.hpp"

NodeText::NodeText(const FontManager& fonts, unsigned int size=12, bool outline=true):
	node_text()
{
	node_text.setFont(fonts.getResourceReference(Fonts_ID::Arial_Narrow));
	node_text.setCharacterSize(size);
	node_text.setFillColor(sf::Color::White);
	if (outline)
	{
		node_text.setOutlineColor(sf::Color::Black);
		node_text.setOutlineThickness(1.f);
	}
}

NodeText::NodeText(const sf::Font& font, const std::string& text = "", unsigned int size = 12, bool outline = true)
{
	node_text.setFont(font);
	node_text.setCharacterSize(size);
	node_text.setFillColor(sf::Color::White);
	if (outline)
	{
		node_text.setOutlineColor(sf::Color::Black);
		node_text.setOutlineThickness(1.f);
	}
}

void NodeText::setString(const std::string& text)
{
	node_text.setString(text);
}

void NodeText::setOutline(float thickness, sf::Color color)
{
	node_text.setOutlineColor(color);
	node_text.setOutlineThickness(thickness);
}

void NodeText::setColor(sf::Color color)
{
	node_text.setFillColor(color);
}

void NodeText::setSize(unsigned int size)
{
	node_text.setCharacterSize(size);
}

void NodeText::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(node_text, states);
}
