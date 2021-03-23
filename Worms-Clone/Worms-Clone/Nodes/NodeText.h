#ifndef NODETEXT_H
#define NODETEXT_H

#include "NodeScene.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"

#include "../Resources/Resources.h"

class NodeText : public NodeScene
{
public:
	// Takes TextureManager to search for a texture
	explicit NodeText(const FontManager& fonts, unsigned int size, bool outline);

	// This allows us to specify a Font and a text
	NodeText(const sf::Font& font, const std::string& text, unsigned int size, bool outline);

	// Function members
	void setString(const std::string&);
	void setOutline(float thickness, sf::Color color);
	void setColor(sf::Color color);
	void setSize(unsigned int size);

private:
	// Typically draw graphical object onto the screen using dynamic-linking
	void drawThis(sf::RenderTarget&, sf::RenderStates) const override;

	sf::Text nodeText;
};

#endif