#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "../State.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "../../Resources/Resources.h"

class StateStack;

class TitleState : public State
{
public:
	TitleState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts);

	virtual void draw() const override;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
	virtual bool update(sf::Time) override;
	virtual bool handleEvent(const sf::Event& event) override;


private:
	sf::Sprite background_sprite;
	sf::Texture background_texture;


	// It tells that user can skip it
	// pressing a button
	sf::Text information;

	// Padding of the text from the corner of the screen
	float text_padding = 15.f;

	sf::RenderWindow* window;
};


#endif