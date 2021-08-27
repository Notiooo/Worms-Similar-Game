#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "Level Editor/Editor.h"
#include "Resources/Resources.h"
#include "States/State.h"

class StateStack;

/**
 * \brief The state in which the player can edit the map inside the game
 */
class EditorState : public State
{
public:
	EditorState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts);

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
	Editor levelEditor; //!< An editor that allows you to create and modify the game world
};


#endif