#ifndef WORMHITSTATE_H
#define WORMHITSTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

class WormHitState : public State
{
public:
	WormHitState(StateStack&, Worm&, TextureManager& textures);

	virtual void draw() const override;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

	virtual bool update(sf::Time) override;
	virtual bool handleEvent(const sf::Event& event) override;
private:
	Worm& worm;
	sf::Sprite hitWorm;

	b2Vec2 savedPosition;
	float velocityToStop = 1;

	sf::Clock offsetClock;
	sf::Time offsetTime = sf::seconds(1);
	bool blocked = false;
	
};

#endif