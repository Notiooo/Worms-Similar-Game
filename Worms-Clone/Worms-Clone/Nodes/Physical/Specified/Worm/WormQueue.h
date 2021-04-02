#ifndef WORMQUEUE_H
#define WORMQUEUE_H
#include <memory>
#include <list>

#include "Worm.h"

/**
 * \brief A team made up of their assigned colour and a list of worms
 */
struct Team
{
	friend bool operator==(const Team& team1, const Team& team2)
	{
		return team1.color == team2.color;
	}
	
    Team(sf::Color color, std::unique_ptr<Worm>& worm): color(color)
    {
        worms.push_back(std::move(worm));
    }
	
	sf::Color color;
	std::list<std::unique_ptr<Worm>> worms;
};


/**
 * \brief A queue of the teams consisting of the worms.
 *
 *				 +--------+currentTeam
 *				 |
 *				 v
 *	+-------------------------+    +-------------------------+
 *	|          Team           |    |          Team           |
 *	|-------------------------|    |-------------------------|
 *	|     sf::Color::Red      |    |     sf::Color::Blue     |
 *	+-------------------------+    +-------------------------+
 *	|Worm, Worm1, Worm2, Worm3|    |Worm, Worm1, Worm2, Worm3|
 *	+-------------------------+    +-------------------------+
 *	  ^
 *	  |
 *	  +
 *	front()
 */
class WormQueue
{
public:

	/**
	 * \brief Add worm to the queue
	 * \param worm Worm to add
	 */
	void addWorm(std::unique_ptr<Worm>& worm);

	/**
	 * \brief Returns the next worm that should play its turn.
	 * \return A reference to a worm that should play
	 *
	 *				   An example of getNextWorm()
	 *
	 *				 +--------+currentTeam
	 *				 |
	 *				 v
	 *	+-------------------------+    +-------------------------+
	 *	|          Team           |    |          Team           |
	 *	|-------------------------|    |-------------------------|
	 *	|     sf::Color::Red      |    |     sf::Color::Blue     |
	 *	+-------------------------+    +-------------------------+
	 *	|Worm, Worm1, Worm2, Worm3|    |Worm, Worm1, Worm2, Worm3|
	 *	+-------------------------+    +-------------------------+
	 *
	 *
	 *						   currentTeam+--------+
	 *											   |
	 *											   v
	 *	+-------------------------+    +-------------------------+
	 *	|          Team           |    |          Team           |
	 *	|-------------------------|    |-------------------------|
	 *	|     sf::Color::Red      |    |     sf::Color::Blue     |
	 *	+-------------------------+    +-------------------------+
	 *	|<--Worm1, Worm2, Worm3   |    |Worm, Worm1, Worm2, Worm3|
	 *	+-+---------------------^-+    +--+----------------------+
	 *	  |                     |         |
	 *	  +------->Worm---------+         v
	 *									Return
	 *		Move worm to the end         this
	 *		
	 */
	Worm& getNextWorm();
	
	/**
	 * \brief Checks that there is no worm in the queue
	 * \return True if there is no worm in the queue, false otherwise
	 */
	bool isEmpty() const;

	// Later it will be made with iterators
	// For now those functions are used to update and draw

	
	/**
	 * \brief Updates the status of worms in the queue
	 * \param deltaTime Time elapsed since last frame
	 */
	void update(sf::Time deltaTime);

	/**
	 * \brief Draws all the worms to the given target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * \brief It takes input (event) from the user and passes to the worms
	 * \param event user input
	 */
	void handleEvents(const sf::Event& event);

	
	/**
	 * \brief Returns the currently played worm
	 * \return Reference to the worm currently being played
	 */
	Worm& front();

	
	/**
     * \brief Removes all worms marked for removal. If their teams are empty, deletes them too
     */
    void removeDestroyed();

	
	/**
	 * \brief Calculates the number of teams currently alive (still playing).
	 * \return Number of teams currently alive
	 */
	int aliveTeams();

private:
	std::list<Team> wormQueue; //!< Queue containing all the Teams with worms
	std::list<Team>::iterator currentTeam = wormQueue.end(); //!< Iterator indicating the currently played team
};

#endif
