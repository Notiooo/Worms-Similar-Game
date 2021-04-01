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
	GUI::Container buttons;
	const FontManager& fonts;
	sf::RenderWindow& window;
	TextureManager textures;
	sf::Sprite backgroundTexture;
	b2World World;
	NodeScene rootScene; //!< It will be used as the live "background"

	sf::Text gameName;
	sf::Text author;

	sf::Text amountText;
	sf::Text amountTeams;
	int& wormsPerTeam;
	int& numberOfTeams;
	int maxWormAmount = 6;
	int minWormAmount = 2;

	int maxTeamsAmount = 4;
	int minTeamsAmount = 2;
	
};


#endif