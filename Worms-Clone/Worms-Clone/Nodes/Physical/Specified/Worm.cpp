#include "Worm.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include <iostream>


Worm::Worm(b2World& world, TextureManager& textures, sf::Vector2f position):
	NodePhysical(world, Physical_Types::Dynamic_Type, position),
	wormSprite(textures.getResourceReference(Textures_ID::AnExamplaryWorm))
{
	// Set origin to the center
	sf::FloatRect boundaries_of_worm = wormSprite.getLocalBounds();
	wormSprite.setOrigin(boundaries_of_worm.width / 2.f, boundaries_of_worm.height / 2.f);

	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox((getSpriteSize().x / 2.f) / B2_SCALAR, 
				   (getSpriteSize().y / 2.f) / B2_SCALAR);

	// Set physical properties of the Worm object
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 1.f;
	FixtureDef.restitution = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);

	// Ensures that this object won't rotate
	Body->SetFixedRotation(true);

	// This hitbox is placed in legs of the worm
	// It will register all collisions to we can
	// know when the worm can jump, and when it is
	// in the air.
	Shape.SetAsBox((getSpriteSize().x /2.f) / B2_SCALAR, (0.5 / 2.f) / B2_SCALAR, 
					b2Vec2(0, (getSpriteSize().y / 2.f) / B2_SCALAR), 0);
	FixtureDef.isSensor = true;

	// It stores our object inside the userData pointer.
	// Before I do it I static_cast it to NodeScene
	// as all object provided to the userData.pointer
	// should be the NodeScene.
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(static_cast<NodeScene*>(this));

	// I create the fixture and attach it to the body
	Body->CreateFixture(&FixtureDef);

	// I also ask the world to listen for the possible
	// collisions with legs of my worm
	world.SetContactListener(&listen_footsteps);
	
}

void Worm::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(wormSprite, states);
}

void Worm::updateThis(sf::Time deltaTime)
{
	// If the Worm's legs collide with the ground
	// Then it should be possible to make a jump
	if (footCollisions)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			Body->SetLinearVelocity({ -movingSpeed, Body->GetLinearVelocity().y });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			Body->SetLinearVelocity({ movingSpeed, Body->GetLinearVelocity().y });
	}

	// Recalculates the sprite so it is in the position of the physical object
	wormSprite.setPosition(B2_SCALAR * Body->GetPosition().x, B2_SCALAR * Body->GetPosition().y);
	wormSprite.setRotation(Body->GetAngle() * 180 / b2_pi);
}

void Worm::handleThisEvents(const sf::Event& event)
{
	// Controls single inputs
	switch (event.type)
	{
		// When key is released, and worm collide with the ground
		// then it velocity should be instantly reduced to zero
		case (sf::Event::KeyReleased):
		{
			if (footCollisions && (
				event.key.code == sf::Keyboard::A ||
				event.key.code == sf::Keyboard::D ||
				event.key.code == sf::Keyboard::W ||
				event.key.code == sf::Keyboard::S))
				Body->SetLinearVelocity({ 0.f, 0.f });
			break;
		}

		// If ANY key is pressed I should
		case (sf::Event::KeyPressed):
		{
			// Rotate the Sprite in according direction
			if (event.key.code == leftButton)
				wormSprite.setScale(std::abs(wormSprite.getScale().x), wormSprite.getScale().y);
			
			//Rotate the Sprite in according direction
			if (event.key.code == sf::Keyboard::D)
				wormSprite.setScale(-std::abs(wormSprite.getScale().x), wormSprite.getScale().y);
			
			// When Worm touches ground we should allow it to make a jump.
			if (footCollisions)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					if (wormSprite.getScale().x > 0)
						Body->ApplyForceToCenter({ -jumpStrength, -3 * jumpStrength }, true);
					else
						Body->ApplyForceToCenter({ jumpStrength, -3 * jumpStrength }, true);
				}
			} 
			break;
		}

	}

}

sf::Vector2f Worm::getSpriteSize()
{
	return sf::Vector2f({wormSprite.getTexture()->getSize().x * wormSprite.getScale().x, 
						 wormSprite.getTexture()->getSize().y * wormSprite.getScale().y});
}

void GroundedListener::BeginContact(b2Contact* contact)
{
	// Takes information about the occured collision
	b2FixtureUserData userData = contact->GetFixtureA()->GetUserData();

	// All collision informations should be stored as NodeScene
	// So I cast it to this object that it supposed to be.
	NodeScene* node = reinterpret_cast<NodeScene*>(userData.pointer);

	// I'm looking for collisions of my worm's feet
	// If it is my worm then it should dynamic_cast
	if (Worm* worm = dynamic_cast<Worm*>(node))
		++(worm->footCollisions);

	// Same thing here for other object in the collision
	userData = contact->GetFixtureB()->GetUserData();
	node = reinterpret_cast<NodeScene*>(userData.pointer);
	if (Worm* worm = dynamic_cast<Worm*>(node))
		++(worm->footCollisions);


}

void GroundedListener::EndContact(b2Contact* contact)
{
	// Takes information about the occured collision
	b2FixtureUserData userData = contact->GetFixtureA()->GetUserData();

	// All collision informations should be stored as NodeScene
	// So I cast it to this object that it supposed to be.
	NodeScene* node = reinterpret_cast<NodeScene*>(userData.pointer);

	// I'm looking for collisions of my worm's feet
	// If it is my worm then it should dynamic_cast
	if (Worm* worm = dynamic_cast<Worm*>(node))
		--(worm->footCollisions);

	// Same thing here for other object in the collision
	userData = contact->GetFixtureB()->GetUserData();
	node = reinterpret_cast<NodeScene*>(userData.pointer);
	if (Worm* worm = dynamic_cast<Worm*>(node))
		--(worm->footCollisions);
}
