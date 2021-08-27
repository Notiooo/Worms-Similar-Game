#ifndef NODEWATER_H
#define NODEWATER_H

#include <SFML/Graphics/Sprite.hpp>

#include "Nodes/Physical/NodePhysicalBody.h"
#include "Resources/Resources.h"

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
		
		sf::Sprite waterLayerSprite; //!< Sprite being a graphic image of a water wave
		float ratioOfMovingSpeed; //!< A factor that determines how fast the waves are moving in this layer.
		float currentLeftPosition = 0.f; //!< Current texture offset defining the wave displacement
	};

	NodeWater(b2World& world, const sf::Texture& texture);

	/**
	 * \brief Sets the size o the object
	 * \param width The new width of the object
	 * \param height The new height of the object
	 */
	void setSize(float width, float height);
	
	/**
	 * \brief Sets the position of the water relative to the centre of the object
	 * \param x The new position on the x-axis
	 * \param y the new position on the y-axis
	 */
	void setPosition(float x, float y);

private:
	
	/**
	 * \brief Draws the different layers of water / waves.
	 */
	void drawThis(sf::RenderTarget&, sf::RenderStates) const override;

	/**
	 * \brief Creates the physical body of an object
	 */
	void createBody();

	/**
	 * \brief It updates the object, synchronises the physical part and takes care of the movement of the waves / water.
	 * \param deltaTime Time elapsed since the last frame
	 */
	void updateThis(sf::Time deltaTime) override;

	// === Waves forming layers of water === //
	WaterLayer waterTopLayer;
	WaterLayer waterMiddleLayer;
	WaterLayer waterBottomLayer;

	sf::Sprite backgroundWater; // Not used yet

	b2Fixture* fixture; //!< Attaches a shape to a body


};

#endif