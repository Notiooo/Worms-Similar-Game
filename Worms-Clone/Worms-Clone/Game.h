#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "States/StateStack.h"

class Game
{
public:
	Game();
	void run();

private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();

	void loadResources();

	static const sf::Time TIME_PER_FRAME;
	sf::RenderWindow gameWindow;

	// All in-game managers
	// I decided that for this moment I don't care to put textures here
	// mostly because I don't want to store whole game in one TextureManager
	// actually some textures between states probably should be deleted 
	// and not be stored in memory for all the time of execution
	//TextureManager textures;

	FontManager fonts;

	// Main stack 
	StateStack appStack;
};


#endif
