#include "Worm.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include <iostream>

// States
#include "../../../../States/States.h"
#include "WormHideState.h"
#include "WormPlayState.h"
#include "WormWaitState.h"


Worm::Worm(b2World& world, TextureManager& textures, sf::Vector2f position):
	NodePhysical(world, Physical_Types::Dynamic_Type, position),
	wormSprite(textures.getResourceReference(Textures_ID::AnExamplaryWorm))
{
	ropeSprite.setTexture(textures.getResourceReference(Textures_ID::Rope));

	// Makes the rope really long (I should in future pass the window y size)
	ropeSprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(ropeSprite.getTexture()->getSize().x, 1000)));

	// Set origin to centered-bottom of the rope
	sf::FloatRect boundaries_of_rope = ropeSprite.getLocalBounds();
	ropeSprite.setOrigin(boundaries_of_rope.width / 2.f, boundaries_of_rope.height);

	// Set origin to the center
	sf::FloatRect boundaries_of_worm = wormSprite.getLocalBounds();
	wormSprite.setOrigin(boundaries_of_worm.width / 2.f, boundaries_of_worm.height / 2.f);

	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox((getSpriteSize(wormSprite).x / 2.f) / B2_SCALAR, 
				   (getSpriteSize(wormSprite).y / 2.f) / B2_SCALAR);

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
	Shape.SetAsBox((getSpriteSize(wormSprite).x /2.f) / B2_SCALAR, (0.5 / 2.f) / B2_SCALAR, 
					b2Vec2(0, (getSpriteSize(wormSprite).y / 2.f) / B2_SCALAR), 0);
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


	// Save states of the worms
	wormStack.saveState<WormHideState>(State_ID::WormHideState, *this);
	wormStack.saveState<WormPlayState>(State_ID::WormPlayState, *this);
	wormStack.saveState<WormWaitState>(State_ID::WormWaitState, *this);

	wormStack.push(State_ID::WormWaitState);
	
}

void Worm::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ropeSprite, states);
	target.draw(wormSprite, states);

	wormStack.draw();
}

void Worm::updateThis(sf::Time deltaTime)
{
	// Update current state of the worm
	wormStack.update(deltaTime);

	// Recalculates the sprite so it is in the position of the physical object
	wormSprite.setPosition(B2_SCALAR * Body->GetPosition().x, B2_SCALAR * Body->GetPosition().y);

	// In theory it is unnecessary as the worm has a fixed rotation
	wormSprite.setRotation(Body->GetAngle() * 180 / b2_pi);

	ropeSprite.setPosition(wormSprite.getPosition());
}

void Worm::handleThisEvents(const sf::Event& event)
{
	wormStack.handleEvent(event);
}

void Worm::activateHideState()
{
	wormStack.clear();
	wormStack.push(State_ID::WormHideState);
}

void Worm::activateWaitState()
{
	wormStack.clear();
	wormStack.push(State_ID::WormWaitState);
}

void Worm::activatePlayState()
{
	wormStack.clear();
	wormStack.push(State_ID::WormPlayState);

}

sf::Vector2f Worm::getSpriteSize(const sf::Sprite& spr)
{
	return sf::Vector2f({spr.getTexture()->getSize().x * spr.getScale().x, 
						 spr.getTexture()->getSize().y * spr.getScale().y});
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
