#include "Worm.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

// States
#include "../../../../States/States.h"
#include "WormHideState.h"
#include "WormPlayState.h"
#include "WormWaitState.h"
#include "../../CollideTypes.h"

// Test purposes
#include "Weapons/Bazooka.h"

Worm::Worm(b2World& world, TextureManager& textures, FontManager& fonts, sf::Vector2f position):
	NodePhysical(world, Physical_Types::Dynamic_Type, position),
	wormSprite(textures.getResourceReference(Textures_ID::AnExamplaryWorm))
{

	// ======= Setup the Sprites ======= //

	ropeSprite.setTexture(textures.getResourceReference(Textures_ID::Rope));

	// Makes the rope really long (I should in future pass the window y size)
	ropeSprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(ropeSprite.getTexture()->getSize().x, 1000)));
	ropeSprite.setPosition(this->getPosition());

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
	sf::FloatRect boundaries_of_name = wormName.getLocalBounds();
	wormName.setOrigin(boundaries_of_name.width / 2.f, boundaries_of_name.height / 2.f);
	wormName.setPosition(this->getPosition().x, this->getPosition().y - 50);


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

	activateWaitState();


	// Test purposes
	inventory.push_back(std::move(std::make_pair(99, std::make_unique<Bazooka>(world, textures))));
	selected_weapon = &inventory.front();
	
}


void Worm::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ropeSprite, states);
	target.draw(wormSprite, states);
	target.draw(wormName, states);
	
	wormStack.draw();
	wormStack.draw(target, states);
}

void Worm::updateThis(sf::Time deltaTime)
{
	// Update current state of the worm
	wormStack.update(deltaTime);


	// ======= Setup the Sprites ======= //
	// Recalculates the sprite so it is in the position of the physical object
	this->setPosition(B2_SCALAR * Body->GetPosition().x, B2_SCALAR * Body->GetPosition().y);

	// In theory it is unnecessary as the worm has a fixed rotation
	this->setRotation(Body->GetAngle() * 180 / b2_pi);
}

void Worm::handleThisEvents(const sf::Event& event)
{
	wormStack.handleEvent(event);
}

void Worm::activateHideState()
{
	current_state = State_ID::WormHideState;
	wormStack.clear();
	wormStack.push(State_ID::WormHideState);
}

void Worm::activateWaitState()
{
	current_state = State_ID::WormWaitState;
	wormStack.clear();
	wormStack.push(State_ID::WormWaitState);
}

void Worm::activatePlayState()
{
	current_state = State_ID::WormPlayState;
	wormStack.clear();
	wormStack.push(State_ID::WormPlayState);

}

State_ID Worm::getCurrentState() const
{
	return current_state;
}

bool Worm::facingRight()
{
	return wormSprite.getScale().x > 0;
}

sf::Vector2f Worm::getSpriteSize(const sf::Sprite& spr)
{
	return sf::Vector2f({spr.getTexture()->getSize().x * spr.getScale().x, 
						 spr.getTexture()->getSize().y * spr.getScale().y});
}
