#include "pch.h"
#include "Nodes/Physical/Specified/Worm/Worm.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Nodes/Physical/NodeRectangularPhysical.h"

// States
#include "Nodes/Physical/CollideTypes.h"
#include "Nodes/Physical/Specified/Worm/WormHideState.h"
#include "Nodes/Physical/Specified/Worm/WormHitState.h"
#include "Nodes/Physical/Specified/Worm/WormPlayState.h"
#include "Nodes/Physical/Specified/Worm/WormWaitState.h"
#include "States/States.h"

// Test purposes
#include "Nodes/Physical/NodePhysicalSprite.h"
#include "Nodes/Physical/Specified/Worm/WormInventoryState.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Bazooka.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Cannon.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Grenade.h"


Worm::Worm(b2World& world, TextureManager& textures, const FontManager& fonts, sf::RenderWindow& window, SoundPlayer& sounds, sf::Vector2f position) :
	NodePhysicalBody(world, Physical_Types::Dynamic_Type, position),
	wormSprite(textures.getResourceReference(Textures_ID::AnExemplaryWorm)),
	deadWorm(textures.getResourceReference(Textures_ID::DeadWorm)),
	soundPlayer(sounds)
{
	wormName.setOutlineThickness(1.5f);
	setName("Unnamed");
	setTeam(sf::Color::White);
	
	// ======= Setup the healthbar ===== //
	healthBar.setSize({ healthBarWidth, healthBarHeight });
	auto boundaries_of_healthBar = healthBar.getLocalBounds();
	healthBar.setOrigin(boundaries_of_healthBar.width / 2.f, boundaries_of_healthBar.height / 2.f);
	healthBar.setFillColor(sf::Color(0, 255, 0, 150)); // a little transparent green
	healthBar.setOutlineThickness(2.f);
	healthBar.setOutlineColor(sf::Color::Black);
	healthBar.setPosition(0, -(getWormSize().y / 2.f + healthBarHeight / 2.f) - 5.f);

	// ======= Setup the Sprites ======= //

	ropeSprite.setTexture(textures.getResourceReference(Textures_ID::Rope));

	// Makes the rope really long (I should in future pass the window y size)
	ropeSprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(ropeSprite.getTexture()->getSize().x, 1000)));
	ropeSprite.setPosition({0, 0});

	// Set origin to centered-bottom of the rope
	const auto boundaries_of_rope = ropeSprite.getLocalBounds();
	ropeSprite.setOrigin(boundaries_of_rope.width / 2.f, boundaries_of_rope.height);

	// Set origin of the Sprite to the center
	const auto boundaries_of_worm = wormSprite.getLocalBounds();
	wormSprite.setOrigin(boundaries_of_worm.width / 2.f, boundaries_of_worm.height / 2.f);


	// ======= Setup the Worm Name ======= //

	// Set Font for the worm name
	wormName.setFont(fonts.getResourceReference(Fonts_ID::ArialNarrow));
	wormName.setCharacterSize(21);
	const sf::FloatRect boundaries_of_name = wormName.getLocalBounds();
	wormName.setOrigin(boundaries_of_name.width / 2.f, boundaries_of_name.height / 2.f);
	wormName.setPosition(0, -(getWormSize().y / 2.f + healthBarHeight + wormName.getCharacterSize()) - 15.f);


	// ======= Setup the Physical Body ======= //

	// Defines its shape
	b2PolygonShape Shape;
	Shape.SetAsBox((getWormSize().x / 2.f) / B2_SCALAR, 
				   (getWormSize().y / 2.f) / B2_SCALAR);

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
	Shape.SetAsBox((getWormSize().x /2.1f) / B2_SCALAR, (10 / 2.f) / B2_SCALAR, 
					b2Vec2(0, (getWormSize().y / 2.f) / B2_SCALAR), 0);
	FixtureDef.isSensor = true;

	// It stores our object inside the userData pointer.
	// Before I do it I static_cast it to NodeScene
	// as all object provided to the userData.pointer
	// should be the NodeScene.
	FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new Collision(CollideTypes::WormFoot, *this));

	// I create the fixture and attach it to the body
	Body->CreateFixture(&FixtureDef);


	// ======= Setup the WormStack States ======= //

	// Save states of the worms
	wormStack.saveState<WormHideState>(State_ID::WormHideState, *this, textures);
	wormStack.saveState<WormPlayState>(State_ID::WormPlayState, *this, textures, soundPlayer);
	wormStack.saveState<WormWaitState>(State_ID::WormWaitState, *this);
	wormStack.saveState<WormHitState>(State_ID::WormHitState, *this, textures);
	wormStack.saveState<WormInventoryState>(State_ID::WormInventoryState, *this, textures, fonts, window);

	activateState(State_ID::WormWaitState);


	// Test purposes
	inventory.emplace_back(std::make_pair(99, std::make_unique<Bazooka>(world, textures, soundPlayer)));
	inventory.emplace_back(std::make_pair(1, std::make_unique<Cannon>(world, textures, soundPlayer)));
	inventory.emplace_back(std::make_pair(1, std::make_unique<Grenade>(world, textures, fonts, soundPlayer)));
	selectedWeapon = &inventory.front();
	
}


void Worm::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ropeSprite, states);

	// These two states independently draw the worm in their own way.
	if(currentState != State_ID::WormHitState && currentState != State_ID::WormPlayState)
		target.draw(wormSprite, states);
	
	target.draw(wormName, states);
	target.draw(healthBar, states);

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


void Worm::activateState(State_ID state)
{
	currentState = state;
	wormStack.clear();
	wormStack.push(state);
}

State_ID Worm::getCurrentState() const noexcept
{
	return currentState;
}

bool Worm::facingRight() const noexcept
{
	return wormSprite.getScale().x > 0;
}

void Worm::setDamage(int _dmg)
{
	health -= _dmg;
	if (health < 0)
		health = 0;
	healthBar.setSize({ healthBarWidth * health/maxHealth, healthBarHeight });
}

void Worm::setName(const std::string& name) 
{
	wormName.setString(name);
	wormName.setOrigin(wormName.getLocalBounds().width / 2.f, wormName.getLocalBounds().height / 2.f);
}

std::string Worm::getName() const noexcept
{
	return wormName.getString();
}

sf::Color Worm::getTeamColor() const noexcept
{
	return teamColor;
}

void Worm::setTeam(sf::Color _teamColor) noexcept
{
	teamColor = _teamColor;
	wormName.setFillColor(teamColor);
	wormName.setOutlineColor(sf::Color(teamColor.r + 80, teamColor.g + 80, teamColor.b + 80, teamColor.a));
}

bool Worm::isDestroyed()
{
	if (health <= 0)
	{
		auto dead_body = std::make_unique<NodePhysicalSprite>(*World, Physical_Types::Dynamic_Type, deadWorm, getPosition());
		getRootNode()->pinNode(std::move(dead_body));
		return true;
	}
	
	return NodeScene::isDestroyed();
}

sf::Vector2f Worm::getWormSize() const
{
	return {wormSprite.getTexture()->getSize().x * wormSprite.getScale().x,
		    wormSprite.getTexture()->getSize().y * wormSprite.getScale().y};
}
