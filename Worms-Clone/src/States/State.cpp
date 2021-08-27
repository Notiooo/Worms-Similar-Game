#include "pch.h"
#include "States/State.h"

#include "States/StateStack.h"

State::State(StateStack& stack): stack(&stack)
{

}

State::~State()
{
}

void State::requestPush(State_ID stateID) const
{
	stack->push(stateID);
}

void State::requestPop() const
{
	stack->pop();
}

void State::requestClear() const
{
	stack->clear();
}
