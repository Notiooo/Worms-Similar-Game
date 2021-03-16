#include "Worm.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include <iostream>

// States
#include "../../../../States/States.h"
#include "WormHideState.h"
#include "WormPlayState.h"
#include "WormWaitState.h"
#include "../../CollideTypes.h"

Worm::Worm(b2World& world, TextureManager& textures, FontManager& fonts, sf::Vector2f position):
	NodePhysical(world, Physical_Types::Dynamic_Type, position),
	wormSprite(textures.getResourceReference(Textures_ID::AnExamplaryWorm))
{

	// ======= Setup the Sprites ======= //

	ropeSprite.setTexture(textures.getResourceReference(Textures_ID::Rope));

	// Makes the rope really long (I should in future pass the window y size)
	ropeSprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(ropeSprite.getTexture()->getSize().x, 1000)));

	// Set origin to centered-bottom of the rope
	sf::FloatRect boundaries_of_rope = ropeSprite.getLocalBounds();
	ropeSprite.setOrigin(boundaries_of_rope.width / 2.f, boundaries_of_rope.height);

	// Set origin of the Sprite to the center
	sf::FloatRect boundaries_of_worm = wormSprite.getLocalBounds();
	wormSprite.setOrigin(boundaries_of_worm.width / 2.f, boundaries_of_worm.height / 2.f);


	// ======= Setup the Worm Name ======= //

	// Set Font for the worm name
	wormName.setFont(fonts.getResourceReference(Fonts_ID::Arial_Narrow));
	wormName.setCharacterSize(21);


	// ======= Setup the Physical Body ======= //

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
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::WormBody, *this));
	Body->CreateFixture(&FixtureDef);

	// Ensures that this object won't rotate
	Body->SetFixedRotation(true);

	// This hitbox is placed in legs of the worm
	// It will register all collisions to we can
	// know when the worm can jump, and when it is
	// in the air.
	Shape.SetAsBox((getSpriteSize(wormSprite).x /2.5f) / B2_SCALAR, (10 / 2.f) / B2_SCALAR, 
					b2Vec2(0, (getSpriteSize(wormSprite).y / 2.f) / B2_SCALAR), 0);
	FixtureDef.isSensor = true;

	// It stores our object inside the userData pointer.
	// Before I do it I static_cast it to NodeScene
	// as all object provided to the userData.pointer
	// should be the NodeScene.
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::WormFoot, *this));
	//FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(static_cast<Collision*>(wormFootCollision));

	// I create the fixture and attach it to the body
	Body->CreateFixture(&FixtureDef);


	// ======= Setup the WormStack States ======= //

	// Save states of the worms
	wormStack.saveState<WormHideState>(State_ID::WormHideState, *this);
	wormStack.saveState<WormPlayState>(State_ID::WormPlayState, *this);
	wormStack.saveState<WormWaitState>(State_ID::WormWaitState, *this);

	wormStack.push(State_ID::WormWaitState);
	
}

Worm::~Worm()
{
	for (b2Fixture* fix = Body->GetFixtureList(); fix; fix = fix->GetNext())
	{
		delete reinterpret_cast<Collision*>(fix->GetUserData().pointer);
	}
}

void Worm::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ropeSprite, states);
	target.draw(wormSprite, states);
	target.draw(wormName, states);

	wormStack.draw();
}

void Worm::updateThis(sf::Time deltaTime)
{
	// Update current state of the worm
	wormStack.update(deltaTime);


	// ======= Setup the Sprites ======= //
	// Recalculates the sprite so it is in the position of the physical object
	wormSprite.setPosition(B2_SCALAR * Body->GetPosition().x, B2_SCALAR * Body->GetPosition().y);

	// In theory it is unnecessary as the worm has a fixed rotation
	wormSprite.setRotation(Body->GetAngle() * 180 / b2_pi);

	ropeSprite.setPosition(wormSprite.getPosition());


	// ======= Setup the Worm Name ======= //
	// Set origin of the name to the center
	// This thing should be optimized later. The origin should be
	// calculated only when the text changes. Not every single frame!!
	sf::FloatRect boundaries_of_name = wormName.getLocalBounds();
	wormName.setOrigin(boundaries_of_name.width / 2.f, boundaries_of_name.height / 2.f);

	// Make the name follow the worm
	wormName.setPosition(wormSprite.getPosition().x, wormSprite.getPosition().y -50);
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

/*
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
*/

/*
void WormListener::BeginContact(b2Contact* contact)
{
	std::cout << "Worm contact" << std::endl;
	// Takes information about the occured collision
	b2FixtureUserData userData1 = contact->GetFixtureA()->GetUserData();
	b2FixtureUserData userData2 = contact->GetFixtureB()->GetUserData();

	// All collision informations should be stored as NodeScene
	// So I cast it to this object that it supposed to be.
	NodeScene* node1 = reinterpret_cast<NodeScene*>(userData1.pointer);
	NodeScene* node2 = reinterpret_cast<NodeScene*>(userData2.pointer);

	// It is usefull to know which of this is Worm
	Worm* worm1 = dynamic_cast<Worm*>(node1);
	Worm* worm2 = dynamic_cast<Worm*>(node2);

	// This means there is collision between two worms
	if (worm1 && worm2)
	{
		std::cout << "Two Worm contact" << std::endl;
		worm1->Body->SetLinearVelocity(-worm1->Body->GetLinearVelocity());
		worm2->Body->SetLinearVelocity(-worm2->Body->GetLinearVelocity());
	}
}

void WormListener::EndContact(b2Contact* contact)
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
*/