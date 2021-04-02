#ifndef WORMQUEUE_H
#define WORMQUEUE_H
#include <memory>
#include <list>

#include "Worm.h"

struct Team
{
    Team(sf::Color color, std::unique_ptr<Worm>& worm): color(color)
    {
        worms.push_back(std::move(worm));
    }
	
	sf::Color color;
	std::list<std::unique_ptr<Worm>> worms;
};

class WormQueue
{
public:

	void addWorm(std::unique_ptr<Worm>& worm);

	Worm& getNextWorm();
	Worm& getPreviousWorm();
	bool isEmpty() const;

	// Later it will be made with iterators
	// For now those functions are used to update and draw
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void handleEvents(const sf::Event& event);
	Worm& front();
    void removeDestroyed();
	int aliveTeams();

private:
	std::list<Team> wormQueue;
	std::list<Team>::iterator currentTeam = wormQueue.end();
};

#endif
