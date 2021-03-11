#include "TitleState.h"

TitleState::TitleState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts):
	State(stack),
	window(&window)
{

	// Loads and sets texture of the background
	background_texture.loadFromFile("Resources/Textures/TitleState/example.png");
	background_sprite.setTexture(background_texture);
	
	// Loads the font
	information.setFont(fonts.getResourceReference(Fonts_ID::Arial_Narrow));

	// Get dimensions of the screen and scales background to the size of the window
	sf::Vector2f screen_dimensions = window.getView().getSize();
	background_sprite.setScale(
		screen_dimensions.x / background_sprite.getLocalBounds().width,
		screen_dimensions.y / background_sprite.getLocalBounds().height);


	// Sets text such that it is white with black outline, and set its position
	// to the right-bottom corner of the screen with some padding
	unsigned int character_size = information.getCharacterSize();
	information.setFillColor(sf::Color::White);
	information.setOutlineColor(sf::Color::Black);
	information.setOutlineThickness(1.f);

	information.setString("Click any button to play");
	information.setPosition(screen_dimensions.x - information.getLocalBounds().width - text_padding,
							screen_dimensions.y - information.getLocalBounds().height - text_padding);
}

void TitleState::draw()
{
	window->draw(background_sprite);
	window->draw(information);
}

bool TitleState::update(sf::Time)
{
	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestPop();
		requestPush(State_ID::GameState);
	}
	return true;
}
