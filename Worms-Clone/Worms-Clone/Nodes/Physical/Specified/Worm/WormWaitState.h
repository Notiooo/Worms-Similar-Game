#ifndef WORMWAITSTATE_H
#define WORMWAITSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

class WormWaitState : public State
{
public:
	WormWaitState(StateStack&, Worm&);

	virtual void draw() const override;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

	virtual bool update(sf::Time) override;
	virtual bool handleEvent(const sf::Event& event) override;
private:
	Worm& worm;
	
	// It says if player can be moved or not
	b2Vec2 saved_position;
};

#endif