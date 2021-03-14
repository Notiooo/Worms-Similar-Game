#ifndef STATE_H
#define STATE_H
#include <memory>
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "States.h"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"


class StateStack;

class State
{
public:
	using Ptr = std::unique_ptr<State>;

	State(StateStack& stack);
	virtual ~State();

	virtual void draw() const = 0;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	virtual bool update(sf::Time) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestPush(State_ID stateID);
	void requestPop();
	void requestClear();

private:
	StateStack* stack;
};

#endif