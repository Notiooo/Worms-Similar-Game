#ifndef WORMHITSTATE_H
#define WORMHITSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"


/**
 * \brief A state in which a worm has been hit by something.
 *
 * This state is responsible for changing the texture that shows the player that
 * this worm has been hit. The character is then also unlocked and can physically
 * be moved. It will only be locked again on contact with the ground and at a
 * sufficiently low speed. A few seconds later, the state will also change back
 * to "WaitState", among other things.
 */
class WormHitState : public State
{
public:
	WormHitState(StateStack&, Worm&, TextureManager& textures);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;

private:
	// === Worm related data === //
	Worm& worm;
	sf::Sprite hitWorm; //!< Sprite that shows the worm has been hit

	// === Position blocking === //
	b2Vec2 savedPosition; //!< A position at which the worm should stay when it is blocked
	float velocityToStop = 1; //!< The velocity needed to block the worm
	bool blocked = false; //!< A flag that holds information if the player position is blocked

	// === State changing ==/
	sf::Clock offsetClock; //!< A clock that measures time is a change of state
	sf::Time offsetTime = sf::seconds(1); //!< The time after which, when a character is blocked, its state is changed to "Waitstate".
	
};

#endif