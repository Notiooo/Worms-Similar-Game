#include "NodeSprite.h"
#include "SFML/Graphics/RenderTarget.hpp"

NodeSprite::NodeSprite(const TextureManager&)
{
}

NodeSprite::NodeSprite(const sf::Texture& texture, const sf::IntRect& rect) :
	nodeSprite(texture, rect)
{

}

void NodeSprite::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nodeSprite, states);
}
