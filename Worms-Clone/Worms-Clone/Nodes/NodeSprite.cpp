#include "NodeSprite.h"
#include "SFML/Graphics/RenderTarget.hpp"

NodeSprite::NodeSprite(const TextureManager&)
{
}

NodeSprite::NodeSprite(const sf::Texture& texture, const sf::IntRect& rect) :
	nodeSprite(texture, rect)
{

}

sf::Vector2f NodeSprite::getSpriteSize() const
{
	return { nodeSprite.getTexture()->getSize().x * nodeSprite.getScale().x,
			 nodeSprite.getTexture()->getSize().y * nodeSprite.getScale().y };
}

void NodeSprite::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nodeSprite, states);
}
