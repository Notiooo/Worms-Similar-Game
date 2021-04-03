#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../State.h"
#include "../../Level Editor/Editor.h"
#include "../../Resources/Resources.h"

class StateStack;

/**
 * \brief The state in which the player can edit the map inside the game
 */
class EditorState : public State
{
public:
	EditorState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;

private:
	sf::RenderWindow& window;
	const FontManager& fonts;
	Editor levelEditor;
};


#endif