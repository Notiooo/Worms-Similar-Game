#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SFML/Graphics/RectangleShape.hpp>

#include "../State.h"
#include "../../GUI/FixedContainer.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "../../Resources/Resources.h"

class StateStack;

/**
 * \brief A pause state in which the states underneath this state in the
 * statestack are blocked against state updates, although still can draw onto the screen.
 */
class PauseState : public State
{
public:
	PauseState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts);

	/**
	 * \brief Draws only this state.
	 */
	void draw() const override;

	/**
	 * \brief Draws only this state to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the status/logic of the state
	 * \param deltaTime the time that has passed since the last frame.
	 */
	bool update(sf::Time deltaTime) override;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	bool handleEvent(const sf::Event& event) override;


private:
	TextureManager textures; //!< Manager containing textures
	sf::RenderWindow& window; //!< Window to which this status is displayed
	
	sf::RectangleShape darkBackground; //!< A dark background that covers the game behind the pause buttons and texts
	sf::Text pausedText; //!< Text displayed when the game is paused
	GUI::FixedContainer containerOfButtons; //!< A container that contains the buttons and moves with the screen
};


#endif