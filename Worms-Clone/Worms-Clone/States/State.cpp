#include "State.h"
#include "StateStack.h"

State::State(StateStack& stack): stack(&stack)
{

}

State::~State()
{
}

void State::requestPush(State_ID stateID)
{
	stack->push(stateID);
}

void State::requestPop()
{
	stack->pop();
}

void State::requestClear()
{
	stack->clear();
}
