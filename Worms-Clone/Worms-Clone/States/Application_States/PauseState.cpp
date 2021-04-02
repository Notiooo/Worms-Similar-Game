#include "PauseState.h"


#include "../../utils.h"
#include "../../GUI/Button.h"

PauseState::PauseState(StateStack& stack, sf::RenderWindow& window, const FontManager& fonts) :
	State(stack),
	window(window),
	darkBackground(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y)),
	pausedText("Pause", fonts.getResourceReference(Fonts_ID::ArialNarrow), 60)
{
	pausedText.setFillColor(sf::Color::White);
	pausedText.setOutlineColor(sf::Color::Black);
	pausedText.setOutlineThickness(1.f);
	pausedText.setPosition(window.getView().getCenter().x, window.getView().getCenter().y - window.getView().getSize().y / 4.f);
	pausedText.setScale(window.getView().getSize().x / window.getDefaultView().getSize().x, window.getView().getSize().y / window.getDefaultView().getSize().y);
	centerOrigin(pausedText);
	
	darkBackground.setFillColor(sf::Color(0,0,0,200));
	centerOrigin(darkBackground);
	
	textures.storeResource(Textures_ID::Paper, "Resources/Textures/World/paper_texture.png");
	textures.getResourceReference(Textures_ID::Paper).setRepeated(true);
	
	auto backToMenuButton = std::make_unique<GUI::Button>(textures, fonts, window);
	backToMenuButton->setText("Exit to the menu");
	backToMenuButton->setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2.f, window.getSize().y / 2.f)));
	backToMenuButton->setScale(window.getView().getSize().x / window.getDefaultView().getSize().x,
		window.getView().getSize().y / window.getDefaultView().getSize().y);
	backToMenuButton->matchSizeToText(10.f);
	backToMenuButton->setActiveFunction([this](GUI::Button& button)
		{
			requestClear();
			requestPush(State_ID::MenuState);
		});

	auto exitGameButton = std::make_unique<GUI::Button>(textures, fonts, window);
	exitGameButton->setText("Exit the game");
	exitGameButton->setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2.f, window.getSize().y / 1.8f)));
	exitGameButton->setScale(window.getView().getSize().x / window.getDefaultView().getSize().x,
		window.getView().getSize().y / window.getDefaultView().getSize().y);
	exitGameButton->matchSizeToText(10.f);
	exitGameButton->setActiveFunction([&window](GUI::Button& button)
		{
			window.close();
		});

	containerOfButtons.store(std::move(backToMenuButton));
	containerOfButtons.store(std::move(exitGameButton));
}

void PauseState::draw() const
{
}

void PauseState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(darkBackground, target.getView().getInverseTransform());
	target.draw(pausedText, states);
	containerOfButtons.draw(target, states);
}

bool PauseState::update(sf::Time)
{
	containerOfButtons.update();

	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	containerOfButtons.handleEvents(event);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestPop();

	return false;
}
