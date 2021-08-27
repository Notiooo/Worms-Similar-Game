#ifndef TITLESTATE_H
#define TITLESTATE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Resources/Resources.h"
#include "States/State.h"

class StateStack;

/**
 * \brief State in which information about game are displayed -- before the main menu is displayed.
 */
class TitleState : public State
{
public:
	TitleState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts);

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
	sf::Sprite backgroundSprite; //!< Main image displayed on the screen
	sf::Texture backgroundTexture; //!< The image that should appear (loaded as texture)
	sf::Text information; //!< Text information about the possibility of moving to the next state
	float textPadding = 15.f; //!< Additional distance from bottom right corner of screen

	sf::RenderWindow* window; //!< Window to which this status is displayed
};


#endif