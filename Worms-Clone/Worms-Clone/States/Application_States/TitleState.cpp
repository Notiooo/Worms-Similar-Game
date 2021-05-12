#include "TitleState.h"

TitleState::TitleState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts):
	State(stack),
	window(&window)
{


	// Loads and sets texture of the background
	backgroundTexture.loadFromFile("Resources/Textures/TitleState/example.png");
	backgroundSprite.setTexture(backgroundTexture);
	
	// Loads the font
	information.setFont(fonts.getResourceReference(Fonts_ID::ArialNarrow));

	// Get dimensions of the screen and scales background to the size of the window
	const auto screenDimensions = window.getView().getSize();
	backgroundSprite.setScale(
		screenDimensions.x / backgroundSprite.getLocalBounds().width,
		screenDimensions.y / backgroundSprite.getLocalBounds().height);


	// Sets text such that it is white with black outline, and set its position
	// to the right-bottom corner of the screen with some padding
	auto characterSize = information.getCharacterSize();
	information.setFillColor(sf::Color::White);
	information.setOutlineColor(sf::Color::Black);
	information.setOutlineThickness(1.f);

	information.setString("Click any button to play");
	information.setPosition(screenDimensions.x - information.getLocalBounds().width - textPadding,
							screenDimensions.y - information.getLocalBounds().height - textPadding);
}

void TitleState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	window->draw(backgroundSprite);
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
		requestPush(State_ID::MenuState);
	}
	return true;
}
