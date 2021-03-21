#ifndef WORMHITSTATE_H
#define WORMHITSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

class WormHitState : public State
{
public:
	WormHitState(StateStack&, Worm&);

	virtual void draw() const override;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

	virtual bool update(sf::Time) override;
	virtual bool handleEvent(const sf::Event& event) override;
private:
	Worm& worm;

	b2Vec2 saved_position;
	float velocity_to_stop = 1;
	
};

#endif