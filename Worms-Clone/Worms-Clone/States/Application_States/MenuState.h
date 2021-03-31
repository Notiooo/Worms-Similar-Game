#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <box2d/box2d.h>
#include <SFML/Graphics/Sprite.hpp>

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
	MenuState(StateStack& stack, const FontManager& fonts, sf::RenderWindow& window);

	void draw() const override;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
	bool update(sf::Time) override;
	bool handleEvent(const sf::Event& event) override;


private:
	void loadResources();
	GUI::Container buttons;
	TextureManager textures;
	sf::Sprite backgroundTexture;
	b2World World;
	NodeScene rootScene; //!< It will be used as the live "background"
	
};


#endif