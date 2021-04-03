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
 * \brief 
 */
class PauseState : public State
{
public:
	PauseState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;


private:
	sf::RectangleShape darkBackground;
	GUI::FixedContainer containerOfButtons;
	sf::RenderWindow& window;
	sf::Text pausedText;

	TextureManager textures;
};


#endif