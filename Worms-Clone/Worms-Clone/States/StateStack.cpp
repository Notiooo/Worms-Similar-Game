#include "StateStack.h"
#include <cassert>

State::Ptr StateStack::createState(State_ID stateID)
{
    auto found = factory.find(stateID);
    assert(found != factory.end());

    // As it find specific recipe in factory that
    // creates a state, then after it is found
    // we call that function to get a pointer to
    // this state, and we return it.
    return found->second();
}

void StateStack::applyChanges()
{
    for (Change change : changesQueue)
    {
        switch (change.operation)
        {
        case Perform::Push:
            stack.push_back(createState(change.stateID)); break;

        case Perform::Pop:
            stack.pop_back(); break;

        case Perform::Clear:
            stack.clear(); break;
        }
    }
    changesQueue.clear();
}

void StateStack::update(sf::Time deltaTime)
{
    // Iterate from the highest state to the lowest state, and stop iterating if
    // any state returns 
    for (auto beg = stack.rbegin(), end = stack.rend(); beg != end; ++beg)
    {
        // If a state is of "Transparent" type, then we iterate further
        // But if it returns false, then we stop iterating. 

        // This allow some states to pause states under it.
        // Like pause for example
        if (!(*beg)->update(deltaTime))
            return;
    }

    applyChanges();
}

void StateStack::draw()
{
    // Drawing starts from the lowest state to the highest state
    for (auto beg = stack.cbegin(), end = stack.cend(); beg != end; ++beg)
        (*beg)->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
    // Iterate from the highest state to the lowest state, and stop iterating if
    // any state returns 
    for (auto beg = stack.rbegin(), end = stack.rend(); beg != end; ++beg)
    {
        // If a state is of "Transparent" type, then we iterate further
        // But if it returns false, then we stop iterating. 

        // This allow some states to pause states under it.
        // Like pause for example
        if (!(*beg)->handleEvent(event))
            return;
    }

    applyChanges();
}


void StateStack::push(State_ID stateID)
{
    changesQueue.push_back(Change{ Perform::Push, stateID });
}

void StateStack::pop()
{
    changesQueue.push_back(Change{ Perform::Pop });
}

void StateStack::clear()
{
    changesQueue.push_back(Change{ Perform::Clear });
}

bool StateStack::empty() const
{
    return changesQueue.empty();
}
