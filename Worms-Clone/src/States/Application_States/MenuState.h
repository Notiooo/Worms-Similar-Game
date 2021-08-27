#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <array>
#include <box2d/box2d.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "GUI/FixedContainer.h"
#include "Nodes/NodeScene.h"
#include "Resources/Resources.h"
#include "Sounds/MusicPlayer.h"
#include "Sounds/SoundPlayer.h"
#include "States/State.h"

class StateStack;

/**
 * \brief The main menu state, where the player can adjust
 * the game settings and start the game.
 *
 * The menu is quite extensive, as it contains its own physical world.
 */
class MenuState : public State
{
public:
	MenuState(StateStack& stack, const FontManager& fonts, sf::RenderWindow& window, int& wormAmount, int& numberOfTeams, MusicPlayer& music);
	
	/**
	 * \brief Creates a live background for the menu
	 * \param pos The position on which the live background is to be created
	 */
	void createBackgroundWorld(sf::Vector2f pos);
	
	/**
	 * \brief Creates a point (spawner) that creates falling grenades.
	 * \param pos The position on which grenades are to spawn.
	 */
	void createGrenades(sf::Vector2f pos);

	/**
	 * \brief Draws only this state to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the status/logic of the state
	 * \param deltaTime the time that has passed since the last frame.
	 */
	bool update(sf::Time deltaTime) override;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	bool handleEvent(const sf::Event& event) override;


private:
	/**
	 * \brief Creates main menu buttons
	 * \param window The window on which the buttons are to be displayed
	 * \param position The position on which the buttons are to be displayed
	 */
	void createButtons(sf::RenderWindow& window, sf::Vector2f position);
	
	/**
	 * \brief Loads all needed resources into the texture manager
	 */
	void loadResources();
	const FontManager& fonts; //!< Manager containing loaded and ready to use fonts
	sf::RenderWindow& window; //!< Window to which this status is displayed
	TextureManager textures; //!< Manager containing the textures available for use
	SoundPlayer soundPlayer; //!< A player that allows to play sounds in the game world
	std::array<sf::Vector2u, 5> availableResolutions; //!< An array of selectable screen resolutions
	std::array<sf::Vector2u, 5>::const_iterator selectedResolution; //!< Currently selected screen resolution
	unsigned fullScreen = 0; //!< Bit value containing information about whether fullscreen is running

	// === Front Menu === //
	GUI::FixedContainer buttons; //!< Includes all buttons inside the menu
	sf::Text gameName; //!< Textual representation of the game title
	sf::Text author; //!< Textual representation of the author of the game
	sf::Text amountText; //!< Text representation of a selected number of worms per team inside the game
	sf::Text amountTeams; //!< Text representation of a selected number of teams inside the game

	// === Game Parameters === //
	int& wormsPerTeam; //!< How many worms are allocated per team
	int& numberOfTeams; //!< How many teams are inside the game
	int maxWormAmount = 6; //!< Maximum number of worms per team
	int minWormAmount = 2; //!< Minimum number of worms per team

	int maxTeamsAmount = 4; //!< Maximum number of teams
	int minTeamsAmount = 2; //!< Minimum number of teams
	
	// === Live Background === //
	sf::Sprite backgroundTexture; //!< Main menu screen background
	b2World World; //!< Physical simulation of the world
	NodeScene rootScene; //!< It will be used as the live background
	
};


#endif