#include "EditorState.h"

#include "../StateStack.h"
#include "SFML/Graphics/RenderWindow.hpp"

EditorState::EditorState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts):
	State(stack),
	window(window),
	fonts(fonts),
	levelEditor(window, fonts)
{
	
}

void EditorState::draw() const
{
	levelEditor.draw();
}

void EditorState::draw(sf::RenderTarget&, sf::RenderStates) const
{
}

bool EditorState::update(sf::Time deltaTime)
{
	levelEditor.update(deltaTime);

	levelEditor.removeDestroyed();

	return false;
}

bool EditorState::handleEvent(const sf::Event& event)
{
	levelEditor.handleEvent(event);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		levelEditor.saveWorld();
		requestPop();
		requestPush(State_ID::MenuState);
	}
	
	return false;
}
