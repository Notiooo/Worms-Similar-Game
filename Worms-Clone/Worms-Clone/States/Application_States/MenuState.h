#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <box2d/box2d.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


#include "../State.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "../../Resources/Resources.h"
#include "../../GUI/Container.h"
#include "../../Nodes/NodeScene.h"

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
	void createBackgroundWorld(sf::Vector2f pos);
	void createGrenades(sf::Vector2f pos);
	MenuState(StateStack& stack, const FontManager& fonts, sf::RenderWindow& window, int& wormAmount, int& numberOfTeams);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;


private:
	void loadResources();
	const FontManager& fonts;
	sf::RenderWindow& window;
	TextureManager textures;

	// === Front Menu === //
	GUI::Container buttons; //!< Includes all buttons inside the menu
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
	sf::Sprite backgroundTexture;
	b2World World;
	NodeScene rootScene; //!< It will be used as the live background
	
};


#endif