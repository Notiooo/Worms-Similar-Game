#ifndef NODEWATER_H
#define NODEWATER_H

#include "NodeScene.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../Resources/Resources.h"
#include "Physical/NodePhysicalBody.h"

/**
 * \brief A very simple (a kind of prototype) node that looks like water.
 * When it comes into contact with a physical object, it removes it from the world.
 */
class NodeWater : public NodePhysicalBody
{
public:
	
	/**
	 * \brief A structure showing the layer and its parameters, such as:
	 * sprite, moving speed or current texture offset.
	 */
	struct WaterLayer
	{
		WaterLayer(const sf::Texture& texture, float ratioSpeed):
			waterLayerSprite(texture), ratioOfMovingSpeed(ratioSpeed) {}
		
		sf::Sprite waterLayerSprite;
		float ratioOfMovingSpeed;
		float currentLeftPosition = 0.f;
	};

	NodeWater(b2World& world, const sf::Texture& texture);

	void setSize(float width, float height);
	void setPosition(float x, float y);


private:
	
	void drawThis(sf::RenderTarget&, sf::RenderStates) const override;

	void createBody();

	void updateThis(sf::Time deltaTime) override;

	WaterLayer waterTopLayer;
	WaterLayer waterMiddleLayer;
	WaterLayer waterBottomLayer;
	sf::Sprite backgroundWater;

	b2Fixture* fixture; //!< Attaches a shape to a body


};

#endif