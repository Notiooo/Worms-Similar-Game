#include "pch.h"
#include "Nodes/Physical/NodeWater.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Nodes/Physical/CollideTypes.h"
#include "Utils/utils.h"


NodeWater::NodeWater(b2World& world, const sf::Texture& texture) :
	NodePhysicalBody(world, Physical_Types::Static_Type, {0.f, 0.f}),
	waterTopLayer(texture, 60.f),
	waterMiddleLayer(texture, 50.f),
	waterBottomLayer(texture, 30.f)

{
	// The layers have different colours (from light to dark blue)
	waterBottomLayer.waterLayerSprite.setColor(sf::Color(110, 110, 255, 255));
	waterMiddleLayer.waterLayerSprite.setColor(sf::Color(180, 180, 255, 255));
	waterTopLayer.waterLayerSprite.setColor(sf::Color(220, 220, 255, 255));

	// All layers should be centred
	centerOrigin(waterBottomLayer.waterLayerSprite);
	centerOrigin(waterMiddleLayer.waterLayerSprite);
	centerOrigin(waterTopLayer.waterLayerSprite);

	
	waterBottomLayer.waterLayerSprite.setPosition(0.f, 0.f);

	// The middle layer is placed above the low layer
	waterMiddleLayer.waterLayerSprite.setPosition(waterBottomLayer.waterLayerSprite.getPosition().x,
	                                              waterBottomLayer.waterLayerSprite.getPosition().y + waterBottomLayer.
	                                              waterLayerSprite.getGlobalBounds().height / 4.f);

	// The top layer is placed above the top layer
	waterTopLayer.waterLayerSprite.setPosition(waterMiddleLayer.waterLayerSprite.getPosition().x,
	                                           waterMiddleLayer.waterLayerSprite.getPosition().y + waterMiddleLayer.
	                                           waterLayerSprite.getGlobalBounds().height / 4.f);

	createBody();
}

void NodeWater::setSize(float width, float height)
{
	// I don't want the waves to repeat in the Y axis, so I don't allow too much height that will bug it.
	auto maxHeight = waterBottomLayer.waterLayerSprite.getGlobalBounds().height;
	height = ((height > maxHeight) ? maxHeight : height);

	// For each water layer
	for (WaterLayer* waterLayer : {&waterBottomLayer, &waterTopLayer, &waterMiddleLayer})
	{
		// I set the new texture size.
		auto& textureRect = waterLayer->waterLayerSprite.getTextureRect();
		waterLayer->waterLayerSprite.setTextureRect(sf::IntRect(textureRect.left, textureRect.top, width, height));

		// I centre it so that again the x and y positions describe the centre of the object
		centerOrigin(waterLayer->waterLayerSprite);
	}


	// I remove the old (different size) physical body and create a new one.
	delete reinterpret_cast<Collision*>(Body->GetFixtureList()[0].GetUserData().pointer);
	Body->DestroyFixture(fixture);
	createBody();
}

void NodeWater::setPosition(float x, float y)
{
	Body->SetTransform(sfVectorToB2Vec(sf::Vector2f(x, y)), Body->GetAngle());
}

void NodeWater::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(waterBottomLayer.waterLayerSprite, states);
	target.draw(waterMiddleLayer.waterLayerSprite, states);
	target.draw(waterTopLayer.waterLayerSprite, states);
}

void NodeWater::updateThis(sf::Time deltaTime)
{
	// Each layer needs to be updated.
	for (WaterLayer* layer : {&waterBottomLayer, &waterMiddleLayer, &waterTopLayer})
	{
		const auto& layerTextureRect = layer->waterLayerSprite.getTextureRect();
		auto& layerLeftPosition = layer->currentLeftPosition;
		auto layerWidth = layer->waterLayerSprite.getGlobalBounds().width;


		// The waves of the water move accordingly (by changing offset of the texture)
		layerLeftPosition += deltaTime.asSeconds() * layer->ratioOfMovingSpeed;
		if (layerLeftPosition > layerWidth)
			layerLeftPosition -= layerWidth;

		// The texture is then updated in this respect
		layer->waterLayerSprite.setTextureRect(sf::IntRect(layerLeftPosition,
		                                                   layerTextureRect.top, layerTextureRect.width,
		                                                   layerTextureRect.height));
	}

	// Synchronize the drawable Rectangle with the physical object
	updatePhysics();
}


void NodeWater::createBody()
{
	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox(waterBottomLayer.waterLayerSprite.getLocalBounds().width / 2.f / B2_SCALAR,
	               waterBottomLayer.waterLayerSprite.getLocalBounds().height / 2.f / B2_SCALAR);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.2f;
	FixtureDef.shape = &Shape;
	FixtureDef.isSensor = true;
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::Water, *this));
	fixture = Body->CreateFixture(&FixtureDef);
}
