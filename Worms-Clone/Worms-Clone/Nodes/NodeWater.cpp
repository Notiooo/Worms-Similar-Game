#include "NodeWater.h"

#include "../utils.h"
#include "Physical/CollideTypes.h"
#include "SFML/Graphics/RenderTarget.hpp"


NodeWater::NodeWater(b2World& world, const sf::Texture& texture) :
	NodePhysicalBody(world, Physical_Types::Static_Type, {0.f, 0.f}),
	waterTopLayer(texture, 60.f),
	waterMiddleLayer(texture, 50.f),
	waterBottomLayer(texture, 30.f)

{
	waterBottomLayer.waterLayerSprite.setColor(sf::Color(110, 110, 255, 255));
	waterMiddleLayer.waterLayerSprite.setColor(sf::Color(180, 180, 255, 255));
	waterTopLayer.waterLayerSprite.setColor(sf::Color(220, 220, 255, 255));

	centerOrigin(waterBottomLayer.waterLayerSprite);
	centerOrigin(waterMiddleLayer.waterLayerSprite);
	centerOrigin(waterTopLayer.waterLayerSprite);

	waterBottomLayer.waterLayerSprite.setPosition(0.f, 0.f);
	waterMiddleLayer.waterLayerSprite.setPosition(waterBottomLayer.waterLayerSprite.getPosition().x, waterBottomLayer.waterLayerSprite.getPosition().y + waterBottomLayer.waterLayerSprite.getGlobalBounds().height / 4.f);
	waterTopLayer.waterLayerSprite.setPosition(waterMiddleLayer.waterLayerSprite.getPosition().x, waterMiddleLayer.waterLayerSprite.getPosition().y + waterMiddleLayer.waterLayerSprite.getGlobalBounds().height / 4.f);

	createBody();
}

void NodeWater::setSize(float width, float height)
{
	float maxHeight = waterBottomLayer.waterLayerSprite.getGlobalBounds().height;
	height = ((height > maxHeight) ? maxHeight : height);
	
	for(WaterLayer* waterLayer : {&waterBottomLayer, &waterTopLayer, &waterMiddleLayer})
	{
		auto& textureRect = waterLayer->waterLayerSprite.getTextureRect();
		waterLayer->waterLayerSprite.setTextureRect(sf::IntRect(textureRect.left, textureRect.top, width, height));
		centerOrigin(waterLayer->waterLayerSprite);
	}

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
	for (WaterLayer* layer : {&waterBottomLayer, &waterMiddleLayer, &waterTopLayer})
	{
		const auto& layerTextureRect = layer->waterLayerSprite.getTextureRect();

		layer->currentLeftPosition += deltaTime.asSeconds() * layer->ratioOfMovingSpeed;

		if (layer->currentLeftPosition > layer->waterLayerSprite.getGlobalBounds().width)
			layer->currentLeftPosition -= layer->waterLayerSprite.getGlobalBounds().width;

		layer->waterLayerSprite.setTextureRect(sf::IntRect(layer->currentLeftPosition,
			layerTextureRect.top, layerTextureRect.width, layerTextureRect.height));
	}

	// Synchronize the drawable Rectangle with the physical object
	updatePhysics();
}


void NodeWater::createBody()
{
	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox(waterBottomLayer.waterLayerSprite.getLocalBounds().width / 2.f / B2_SCALAR, waterBottomLayer.waterLayerSprite.getLocalBounds().height / 2.f / B2_SCALAR);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.2f;
	FixtureDef.shape = &Shape;
	FixtureDef.isSensor = true;
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::Water, *this));
	fixture = Body->CreateFixture(&FixtureDef);
}
