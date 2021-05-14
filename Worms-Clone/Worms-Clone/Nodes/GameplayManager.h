#ifndef GAMEPLAYMANAGER_H
#define GAMEPLAYMANAGER_H

#include "NodeScene.h"
#include <deque>


#include "../GUI/Container.h"
#include "../GUI/FixedContainer.h"
#include "../Sounds/SoundPlayer.h"
#include "Physical/Specified/Worm/Worm.h"
#include "Physical/Specified/Worm/WormQueue.h"

/**
 * \brief It handles the gameplay rules such as the order of moves
 * -- including the worm queue, and even the countdown timer for a
 * given round or state.
 */
class GameplayManager final : public NodeScene
{
public:
	GameplayManager(b2World& physicalWorld, TextureManager& textures, FontManager& fonts, sf::RenderWindow& window, SoundPlayer& sounds);

	/**
	 * \brief Creates a worm inside the game
	 * \param name Name of the worm
	 * \param teamColor The team of the worm, and at the same time its colour.
	 * \param position Position of the worm on the map
	 */
	void addWorm(const std::string& name, sf::Color teamColor, sf::Vector2f position);

	/**
	 * \brief Draws worms and other pinned nodes
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates worms and other pinned nodes
	 * \param deltaTime Time passed since the last frame
	 */
	void updateThis(sf::Time deltaTime) override;

	/**
	 * \brief Passes on to worms and other pinned nodes the individual inputs made by the player
	 * \param event user input
	 */
	void handleThisEvents(const sf::Event& event) override;

	/**
	 * \brief Checks that pinned knots and worms should be removed.
	 */
	void removeDestroyed() override;

	/**
	 * \brief Displays text below the timer for given amount of time
	 * \param text Text to display
	 * \param color Color of the text
	 * \param time How much time should it stay on the screen. If not set then it stays infinitely long
	 */
	void setWorldMessage(const std::string& text, sf::Color color = sf::Color::White, sf::Time time = sf::Time::Zero);

	/**
	 * \brief Adds an additional amount of time to the current state.
	 * \param time time to add
	 */
	void addTime(sf::Time time);

	/**
	 * \brief It checks if any bullet has been pinned to the scene.
	 * \return True if there is any bullet pinned to the scene, false otherwise
	 */
	bool anyBullet() const;

	/**
	 * \brief Returns itself as RootNode. This ensures that any bullets are assigned to this node.
	 * \return Itself (GameplayManager)
	 */
	NodeScene* getRootNode() override;

	/**
	 * \brief Returns itself as RootNode. This ensures that any bullets are assigned to this node.
	 * \return Itself (GameplayManager)
	 */
	const NodeScene* getRootNode() const override;

	/**
	 * \brief Checks whether the game has already finished 
	 * \return True if the game has finished, false otherwise
	 */
	bool isGameFinished() const noexcept;

private:
	/**
	 * \brief It controls a turn-based game system.
	 * Responsible for adhering to the rules of the game and its flow.
	 */
	void updateTurn();

	/**
	 * \brief Checks whether the game should end already
	 */
	void updateGameResult();

	/**
	 * \brief Sets the game state as finished
	 */
	void setGameFinished();


	b2World& physicalWorld; //!< Physical simulation of the world
	TextureManager& textures; //!< Manager with loaded textures
	FontManager& fonts; //!< Manager with loaded fonts
	sf::RenderWindow& window; //!< Window to which the game is displayed
	SoundPlayer& sounds; //!< A player that allows to play sounds in the game world
	
	// Variables used to control the turn-based game system.

	// ==== Timer ==== //
	sf::Clock roundClock; //!< Timer to control game time
	sf::Time timePerTurn = sf::seconds(30); //!< Time allowed per turn
	sf::Time timePerHide = sf::seconds(5); //!< Time allowed for escape during turn change
	sf::Text roundTimeText; //!< Text that displays the current time on the screen

	// === Game Message === //
	sf::Text gameMessageText; //!< Text that displays below the timer
	sf::Clock gameMessageClock; //!< Used to measure the time at which text should disappear
	sf::Time gameMessageTime; //!< Time for how long the game message should appear on the screen. Zero if infinitely many time.

	
	WormQueue wormQueue; //!< Order in which particular worms can play
	sf::Time additionalTime = sf::Time::Zero; //!< Additional time until the end of the current turn
	bool gameFinished = false; //!< Flag indicating whether the game has ended

	sf::Time leaveGameTime = sf::seconds(5); //!< Time after which player (after win) will be redirected to main menu
};


#endif