#ifndef NODESPRITE_H
#define NODESPRITE_H

#include "NodeScene.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../Resources/Resources.h"

class NodeSprite : public NodeScene
{
public:
	// Takes TextureManager to search for a texture
	explicit NodeSprite(const TextureManager&);

	// Takes TextureManager to search for a texture and uses given IntRect boundaries
	// to display only this part of the Texture
	NodeSprite(const sf::Texture&, const sf::IntRect&);

private:
	// Typically draw graphical object onto the screen using dynamic-linking
	void drawThis(sf::RenderTarget&, sf::RenderStates) const override;

	sf::Sprite nodeSprite;
};

#endif