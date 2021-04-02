#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "../State.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "../../Resources/Resources.h"

class StateStack;

/**
 * \brief State in which information about game are displayed -- before the main menu is displayed.
 */
class TitleState : public State
{
public:
	TitleState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;


private:
	sf::Sprite background_sprite; //!< Main image displayed on the screen
	sf::Texture background_texture; //!< The image that should appear (loaded as texture)
	sf::Text information; //!< Text information about the possibility of moving to the next state
	float text_padding = 15.f; //!< Additional distance from bottom right corner of screen

	sf::RenderWindow* window;
};


#endif