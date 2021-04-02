#ifndef NODEPHYSICALSPRITE_H
#define NODEPHYSICALSPRITE_H

#include "NodePhysicalBody.h"
#include "SFML/Graphics/Sprite.hpp"

/**
 * \brief A node that has its own graphical representation and at the same
 * time is a physical object inside the game world.
 */
class NodePhysicalSprite : public NodePhysicalBody
{
public:
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture, sf::Vector2f position);
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture, sf::Vector2f position, const sf::IntRect& rect);
	NodePhysicalSprite(b2World& world, Physical_Types physical_type, sf::Texture& texture, sf::Vector2f position, const sf::Vector2f& size);
	
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;

	/**
	 * \brief Sets the texture size and enhances the body
	 * inside the simulation adjusting it to the new dimensions.
	 */
	void setSize(const sf::IntRect&);

private:
	/**
	 * \brief It creates the physical body of the object and links it to a physical simulation of the world.
	 */
	void createBody();

protected:
	sf::Sprite sprite; //!< Graphical representation of the object
	b2Fixture* fixture; //!< Attaches a shape to a body

};
#endif