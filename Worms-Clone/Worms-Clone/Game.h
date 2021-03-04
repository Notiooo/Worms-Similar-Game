#ifndef GAME_H
#define GAME_H

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "box2d/box2d.h"

#include "World.h"

class Game
{
public:
	Game();
	void run();

private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();

	static const sf::Time time_per_frame;
	sf::RenderWindow gameWindow;

	World gameWorld;
};





#endif 