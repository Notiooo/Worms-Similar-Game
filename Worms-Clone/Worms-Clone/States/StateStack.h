#ifndef STATESTACK_H
#define STATESTACK_H

#include <map>
#include <functional>
#include <utility>
#include <vector>

#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

#include "State.h"
#include "States.h"

class StateStack : private sf::NonCopyable
{
public:
	// Default constructor
	StateStack() = default;

	// All actions the stack should perform
	enum class Perform
	{
		Push,
		Pop,
		Clear,
	};

	// Creates callable function that might generate
	// the State object providing the given args to its constructor
	template <typename State, typename... Args>
	void saveState(State_ID stateID, Args&&...);

	// Typical cyclic functions in NodeScene system
	void update(sf::Time deltaTime);
	void draw();
	void handleEvent(const sf::Event& event);

	// Operations typical for a stack
	void push(State_ID stateID);
	void pop();
	void clear();
	bool empty() const;

private:

	// Waiting change in the queue
	// For example:
	// operation = Perform::Push
	// State_ID State_ID::wormState
	struct Change
	{
		Perform operation;
		State_ID stateID;
	};

	// Generates a State
	State::Ptr createState(State_ID stateID);

	// Handles the changes in the
	// changes queue
	void applyChanges();

	// Holds all the states
	std::vector<State::Ptr> stack;

	// Holds all the future changes to the stack
	std::vector<Change> changesQueue;

	// A factory generating corresponding States
	// based on their State_ID
	std::map<State_ID, std::function<State::Ptr()>> factory;
};

template <typename State, typename... Args>
inline void StateStack::saveState(State_ID stateID, Args&&... args)
{
	// A factory that allows me to create any state
	// that ID is passed to this function.

	// This allows us to not create all states at once
	// and thanks to this avoid unnecesserily big containers
	// holding all those states that might be never used

	// context holds all the variables and data that state
	// might need to use

	// By default it of course just pass an information
	// to the factory how to create such an object
	// nothing is yet created
	factory[stateID] = [this]()
	{
		return State::Ptr(std::make_unique<State>(*this, args...));
	};
}

#endif 
