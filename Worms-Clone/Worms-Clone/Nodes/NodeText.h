#ifndef NODETEXT_H
#define NODETEXT_H

#include "NodeScene.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"

#include "../Resources/Resources.h"

/**
 * \brief Node that displays text on the screen
 */
class NodeText : public NodeScene
{
public:
	
	/**
	 * \brief Creates a node representing text with given parameters
	 * \param font The font to be used when displaying the text
	 * \param size Size in pixels to be used when displaying the text
	 * \param outline True if the text should have an outline, false otherwise
	 */
	NodeText(const sf::Font& font, unsigned int size, bool outline);

	/**
	 * \brief Creates a node representing text with given parameters
	 * \param text Text that should be displayed
	 * \param font The font to be used when displaying the text
	 * \param size Size in pixels to be used when displaying the text
	 * \param outline True if the text should have an outline, false otherwise
	 */
	NodeText(const sf::Font& font, const std::string& text, unsigned int size, bool outline);

	// === Setters === //
	
	/**
	 * \brief Sets the specified text
	 * \param text Text to set
	 */
	void setString(const std::string& text);

	
	/**
	 * \brief Sets a border for text of a given color
	 * \param thickness Thickness of the outline
	 * \param color Color of the outline
	 */
	void setOutline(float thickness, sf::Color color);
	
	/**
	 * \brief Sets the text color
	 * \param color Color of the text
	 */
	void setColor(sf::Color color);

	
	/**
	 * \brief Sets the text size
	 * \param size Size of the text in pixels
	 */
	void setSize(unsigned int size);

private:

	/**
	 * \brief Draws only this node to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Text nodeText; //!< Graphic representation of text to be displayed on screen
};

#endif