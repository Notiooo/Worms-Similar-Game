#include "GameplayManager.h"

#include "Physical/Specified/Worm/Weapons/Bullet.h"

#include "../utils.h"
#include "../GUI/Button.h"
#include "Physical/Specified/Worm/Weapons/Hitbox.h"

GameplayManager::GameplayManager(b2World& _physicalWorld, TextureManager& _textures, FontManager& _fonts,
                                 sf::RenderWindow& _window, SoundPlayer& _sounds) :
	physicalWorld(_physicalWorld),
	textures(_textures),
	fonts(_fonts),
	window(_window),
	sounds(_sounds)
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
	auto worm = std::make_unique<Worm>(physicalWorld, textures, fonts, window, sounds, position);
	worm->setName(name);
	worm->setTeam(teamColor);

	if (wormQueue.isEmpty())
		worm->activateState(State_ID::WormPlayState);

	// Worm need this node as a parent to properly calculate transform
	// as also it will use this node to get through it to the root node
	// during creating a bullet -- which is object pinned to the root node
	worm->parent = this;
	wormQueue.addWorm(worm);
}

void GameplayManager::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	wormQueue.draw(target, states);

	target.draw(roundTimeText);
	target.draw(gameMessageText);
}

void GameplayManager::updateThis(sf::Time deltaTime)
{
	wormQueue.update(deltaTime);

	updateGameResult();
	updateTurn();

	// The game time and in-game messages should always be in the middle of the game window, so they are set on the fly.
	roundTimeText.setPosition(
		window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, roundTimeText.getLocalBounds().height)));

	gameMessageText.setPosition(
		window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, gameMessageText.getLocalBounds().height * 2.5)));

	// After a period of time, the message should disappear.
	if (gameMessageTime != sf::Time::Zero && gameMessageClock.getElapsedTime() > gameMessageTime)
		setWorldMessage("");
}

void GameplayManager::handleThisEvents(const sf::Event& event)
{
	//for (const auto& worm : wormQueue)
	//	worm->handleEvents(event);
	wormQueue.handleEvents(event);

	if (event.type == sf::Event::MouseWheelScrolled)
	{
		// Fixes the position of the timer and world message
		roundTimeText.setPosition(
			window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2,
			                                     roundTimeText.getCharacterSize())));

		// Current zoom of the window
		const auto currentScale = sf::Vector2f(window.getView().getSize().x / window.getDefaultView().getSize().x,
		                                 window.getView().getSize().y / window.getDefaultView().getSize().y);

		// Updates all fixed text to stay at proper scale
		roundTimeText.setScale(currentScale);
		gameMessageText.setScale(currentScale);
	}
}

void GameplayManager::removeDestroyed()
{
	NodeScene::removeDestroyed();

	wormQueue.removeDestroyed();
}

void GameplayManager::setWorldMessage(const std::string& text, sf::Color color, sf::Time time)
{
	gameMessageText.setString(text);
	gameMessageText.setFillColor(color);
	gameMessageText.setOutlineColor(sf::Color(color.r + 80, color.g + 80, color.b + 80, color.a));
	gameMessageText.setOrigin(gameMessageText.getLocalBounds().width / 2.f,
	                          gameMessageText.getLocalBounds().height / 2.f);

	gameMessageTime = time;
	gameMessageClock.restart();
}

void GameplayManager::addTime(sf::Time time)
{
	additionalTime = time;
}

bool GameplayManager::anyBullet() const
{
	return std::any_of(pinnedNodes.cbegin(), pinnedNodes.cend(), [](const Node& node)
	{
		return (static_cast<bool>(dynamic_cast<Bullet*>(node.get())) || static_cast<bool>(dynamic_cast<Hitbox*>(node.
			get())));
	});
}

NodeScene* GameplayManager::getRootNode()
{
	return this;
}

const NodeScene* GameplayManager::getRootNode() const
{
	return this;
}

bool GameplayManager::isGameFinished() const noexcept
{
	return gameFinished;
}

void GameplayManager::updateTurn()
{
	// On default the game should stay with PlayState for some worm
	static auto hideState = false;

	// The game should go on if there are worms in the queue
	if (!isGameFinished() && !wormQueue.isEmpty())
	{
		auto timeElapsed = roundClock.getElapsedTime();

		// The game should allow to play another worm if
		// a) The time has passed
		// b) The current worm shoot, and now is in the HideState
		if (!hideState && ((timeElapsed > timePerTurn + additionalTime) ||
			(wormQueue.front().getCurrentState() == State_ID::WormHideState)))
		{
			hideState = true;
			additionalTime = sf::Time::Zero;
			setWorldMessage("HIDE!", sf::Color::Red, timePerHide);

			// If player did not shoot, then switch him to HideState
			if (wormQueue.front().getCurrentState() == State_ID::WormPlayState)
				wormQueue.front().activateState(State_ID::WormHideState);

			// Red color stresses the player
			roundTimeText.setFillColor(sf::Color::Red);
			roundClock.restart();
			timeElapsed = sf::Time::Zero;
		}

		if (hideState && (timeElapsed > timePerHide + additionalTime) && !anyBullet())
		{
			hideState = false;
			additionalTime = sf::Time::Zero;

			// If the HideState is over I need to change the current
			// worm state to WaitState, and move it at the back of the queue.
			// Next switch the worm at the front to the PlayState
			wormQueue.front().activateState(State_ID::WormWaitState);
			wormQueue.getNextWorm().activateState(State_ID::WormPlayState);

			auto currentTeam = wormQueue.front().getTeamColor();

			setWorldMessage("Lets go, " + wormQueue.front().getName() + "!", sf::Color::White, sf::seconds(2));

			// Changes Red to White Color
			roundTimeText.setFillColor(sf::Color::White);
			roundClock.restart();
		}

		// Displays time in decreasing order
		sf::Time timeDisplay = ((hideState) ? timePerHide : timePerTurn);
		timeDisplay = timeDisplay + additionalTime - timeElapsed;
		const auto timeSeconds = static_cast<int>(timeDisplay.asSeconds());
		if (timeSeconds >= 0)
			roundTimeText.setString(std::to_string(timeSeconds));
		roundTimeText.setOrigin(roundTimeText.getLocalBounds().width / 2.f,
		                        roundTimeText.getLocalBounds().height / 2.f);
	}
	else
	{
		hideState = false;

		const auto timeElapsed = roundClock.getElapsedTime();
		const auto timeDisplay = leaveGameTime - timeElapsed;
		const auto timeSeconds = static_cast<int>(timeDisplay.asSeconds());
		if (timeSeconds >= 0)
		{
			roundTimeText.setString("You'll be redirected to menu in " + std::to_string(timeSeconds) + " seconds");
			centerOrigin(roundTimeText);
		}
		else
			setDestroyed();
	}
}

void GameplayManager::updateGameResult()
{
	if (isGameFinished())
		return;

	const auto numberOfAliveTeams = wormQueue.aliveTeams();

	if (numberOfAliveTeams == 0)
	{
		setGameFinished();
		setWorldMessage("The game ended in a draw!");
	}
	else if (numberOfAliveTeams == 1)
	{
		setGameFinished();
		std::string winner;

		auto teamColor = wormQueue.front().getTeamColor();

		// TODO: Wrong way to do this! I have to change it later.
		if (teamColor == sf::Color::Red)
			winner = "red";
		else if (teamColor == sf::Color::Blue)
			winner = "blue";
		else if (teamColor == sf::Color::Green)
			winner = "green";
		else if (teamColor == sf::Color::Magenta)
			winner = "magneta";
		else if (teamColor == sf::Color::Yellow)
			winner = "yellow";
		else
			winner = "unknown color";

		setWorldMessage("The game has been won by " + winner);
	}
}

void GameplayManager::setGameFinished()
{
	gameFinished = true;
	roundClock.restart();
	roundTimeText.setString("");
}
