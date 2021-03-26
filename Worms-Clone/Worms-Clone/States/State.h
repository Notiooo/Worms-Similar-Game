#ifndef STATE_H
#define STATE_H
#include <memory>
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "States.h"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"


class StateStack;

/**
 * \brief  The state that is on the stack performs the various
 * functions of updating, drawing and handling user inputs.
 */
class State
{
public:
	using Ptr = std::unique_ptr<State>;
	
	State(StateStack& stack);
	virtual ~State();


	/**
	 * \brief Draws only this state.
	 */
	virtual void draw() const = 0;

	/**
	 * \brief Draws only this state to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	/**
	 * \brief Updates the logic of this state
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	virtual bool update(sf::Time deltaTime) = 0;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	/**
	 * \brief The state will be pushed out in the next iteration of the stack.
	 * \param stateID Identifier of the state to be pushed
	 */
	void requestPush(State_ID stateID);

	
	/**
	 * \brief The state on the top of the stack will be removed in
	 * the next iteration of the stack.
	 */
	void requestPop();

	
	/**
	 * \brief All states on the stack will be removed in the
	 * next iteration of the stack.
	 */
	void requestClear();

private:
	/**
	 * \brief Pointer to the stack containing this state
	 */
	StateStack* stack;
};

#endif