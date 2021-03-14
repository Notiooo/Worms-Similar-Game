#ifndef WORMPLAYSTATE_H
#define WORMPLAYSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

class WormPlayState : public State
{
public:
	WormPlayState(StateStack&, Worm&);

	virtual void draw() const override;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

	virtual bool update(sf::Time) override;
	virtual bool handleEvent(const sf::Event& event) override;
private:
	Worm& worm;
};
#endif