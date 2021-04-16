#ifndef NODESPRITE_H
#define NODESPRITE_H

#include "NodeScene.h"
#include "SFML/Graphics/Sprite.hpp"

/**
 * \brief Node containing and displaying a sprite in addition
 */
class NodeSprite : public NodeScene
{
public:
	/**
	 * \brief Texture to be displayed
	 * \param texture Creates a sprite with given texture
	 */
	explicit NodeSprite(const sf::Texture& texture);
	
	/**
	 * \brief Creates a sprite with given texture and given boundaries
	 * \param texture Texture to be displayed
	 * \param rect boundaries to display only given part of the Texture
	 */
	NodeSprite(const sf::Texture& texture, const sf::IntRect& rect);

	/**
	 * \brief Calculates and returns the dimensions of the sprite
	 * \return Dimensions of the sprite
	 */
	sf::Vector2f getSpriteSize() const;

private:
	/**
	 * \brief Draws only this sprite to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite nodeSprite; //!< Sprite representing the given texture
};

#endif