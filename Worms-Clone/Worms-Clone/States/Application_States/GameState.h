#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../State.h"
#include "../../World.h"

class StateStack;

class GameState : public State
{
public:
	GameState(StateStack& stack, sf::RenderWindow& window);

	virtual void draw() override;
	virtual bool update(sf::Time) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	World gameWorld;
};


#endif