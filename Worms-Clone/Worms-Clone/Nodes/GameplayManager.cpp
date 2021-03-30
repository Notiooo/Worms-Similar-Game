#include "GameplayManager.h"


GameplayManager::GameplayManager(b2World& _physicalWorld, TextureManager& _textures, FontManager& _fonts,
	sf::RenderWindow& _window):
	physicalWorld(_physicalWorld),
	textures(_textures),
	fonts(_fonts),
	window(_window)
{
	// Set up the round timer
	roundTimeText.setFont(fonts.getResourceReference(Fonts_ID::ArialNarrow));
	roundTimeText.setOutlineThickness(1.f);
	roundTimeText.setPosition(window.getView().getSize().x / 2, roundTimeText.getCharacterSize());

	// Set up the message below timer
	gameMessageText.setFont(fonts.getResourceReference(Fonts_ID::ArialNarrow));
	gameMessageText.setOutlineThickness(1.f);
}

void GameplayManager::addWorm(const std::string& name, sf::Color teamColor, sf::Vector2f position)
{
	std::unique_ptr<Worm> worm = std::make_unique<Worm>(physicalWorld, textures, fonts, window, position);
	worm->setName(name);
	worm->setTeam(teamColor);

	if (wormQueue.empty())
		worm->activateState(State_ID::WormPlayState);

	// Worm need this node as a parent to properly calculate transform
	// as also it will use this node to get through it to the root node
	// during creating a bullet -- which is object pinned to the root node
	worm->parent = this;
	wormQueue.push_back(std::move(worm));
}

void GameplayManager::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(auto beg = wormQueue.crbegin(), end = wormQueue.crend(); beg != end; ++beg)
		(*beg)->draw(target, states);

	target.draw(roundTimeText);
	target.draw(gameMessageText);
}

void GameplayManager::updateThis(sf::Time deltaTime)
{
	for (const auto& worm : wormQueue)
		worm->update(deltaTime);

	checkTurnTime();
	roundTimeText.setPosition(
		window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2,
		                                          roundTimeText.getCharacterSize())));

	gameMessageText.setPosition(roundTimeText.getPosition() + sf::Vector2f(0, gameMessageText.getLocalBounds().height + 10));
	if(gameMessageTime != sf::Time::Zero && gameMessageClock.getElapsedTime() > gameMessageTime)
		setWorldMessage("");
}

void GameplayManager::handleThisEvents(const sf::Event& event)
{
	for (const auto& worm : wormQueue)
		worm->handleEvents(event);

	if (event.type == sf::Event::MouseWheelScrolled)
	{

		// Fixes the posiiton of the timer
		roundTimeText.setPosition(
			window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2,
			                                          roundTimeText.getCharacterSize())));

		// Fixes size of the timer
		auto current_scale = sf::Vector2f(window.getView().getSize().x / window.getDefaultView().getSize().x,
												window.getView().getSize().y / window.getDefaultView().getSize().y);
		roundTimeText.setScale(current_scale);
		gameMessageText.setScale(current_scale);
	}
}

void GameplayManager::removeDestroyed()
{
	NodeScene::removeDestroyed();

	auto removal_mark = std::remove_if(wormQueue.begin(), wormQueue.end(), std::mem_fn(&NodeScene::isDestroyed));
	wormQueue.erase(removal_mark, wormQueue.end());

	for (const auto& worm : pinnedNodes)
		worm->removeDestroyed();
}

void GameplayManager::setWorldMessage(const std::string& text, sf::Color color, sf::Time time)
{
	gameMessageText.setString(text);
	gameMessageText.setFillColor(color);
	gameMessageText.setOutlineColor(sf::Color(color.r + 80, color.g + 80, color.b + 80, color.a));
	gameMessageText.setOrigin(gameMessageText.getLocalBounds().width / 2.f, gameMessageText.getLocalBounds().height / 2.f);

	gameMessageTime = time;
	gameMessageClock.restart();
}

void GameplayManager::checkTurnTime()
{
	// On default there is no HideState
	static bool hideState = false;
	if (!wormQueue.empty())
	{
		sf::Time timeElapsed = roundClock.getElapsedTime();

		if (!hideState && ((timeElapsed > timePerTurn) || (wormQueue.front()->getCurrentState() ==
			State_ID::WormHideState)))
		{
			// Redundancy
			hideState = true;
			setWorldMessage("HIDE!", sf::Color::Red, timePerHide);
			
			if (!(wormQueue.front()->getCurrentState() == State_ID::WormHideState))
				wormQueue.front()->activateState(State_ID::WormHideState);

			roundTimeText.setFillColor(sf::Color::Red);
			roundClock.restart();
			timeElapsed = sf::Time::Zero;
		}

		if (hideState && (timeElapsed > timePerHide))
		{
			std::unique_ptr<Worm> worm = std::move(wormQueue.front());
			worm->activateState(State_ID::WormWaitState);
			wormQueue.pop_front();
			wormQueue.push_back(std::move(worm));
			wormQueue.front()->activateState(State_ID::WormPlayState);
			setWorldMessage("Lets go, " + wormQueue.front()->getName() + "!", sf::Color::White, sf::seconds(2));

			// Redundancy
			hideState = false;
			roundTimeText.setFillColor(sf::Color::White);
			roundClock.restart();
		}

		// Wrong way to do this!!! Too many setString (need to optimize it later)
		sf::Time timeDisplay;
		if (hideState)
			timeDisplay = timePerHide - timeElapsed;
		else
			timeDisplay = timePerTurn - timeElapsed;
		
		roundTimeText.setString(std::to_string(static_cast<int>(timeDisplay.asSeconds())));
		roundTimeText.setOrigin(roundTimeText.getLocalBounds().width / 2.f, roundTimeText.getLocalBounds().height / 2.f);
	}
}
