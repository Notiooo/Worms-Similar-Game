#ifndef WORMWAITSTATE_H
#define WORMWAITSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

class WormWaitState : public State
{
public:
	WormWaitState(StateStack&, Worm&);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;
private:
	Worm& worm;
	
	/**
	 * \brief Current position of the player.
	 *
	 * The saved is used to block the movement
	 * of the character so that no physical object
	 * can move it.
	 */
	b2Vec2 savedPosition;
};

#endif