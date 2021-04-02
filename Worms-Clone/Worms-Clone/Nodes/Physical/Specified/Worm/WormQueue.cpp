#include "WormQueue.h"
#include <iostream>

void WormQueue::addWorm(std::unique_ptr<Worm>& worm)
{
	sf::Color teamColor = worm->getTeam();
	auto found = std::find_if(wormQueue.begin(), wormQueue.end(), [teamColor](Team& team)
		{
			return (team.color == teamColor);
		});
	 
	if (found == wormQueue.end())
		wormQueue.emplace_back(teamColor, worm);
	else
		found->worms.push_back(std::move(worm));

	if (wormQueue.size() == 1)
		currentTeam = wormQueue.begin();
}

Worm& WormQueue::getNextWorm()
{
	auto previousTeam = currentTeam;
	++currentTeam;

	if (previousTeam->worms.size() > 1)
	{
		std::unique_ptr<Worm> wormPtr = std::move(previousTeam->worms.front());
		previousTeam->worms.pop_front();
		previousTeam->worms.push_back(std::move(wormPtr));
	}

	if(currentTeam == wormQueue.end())
		currentTeam = wormQueue.begin();
	
	return *currentTeam->worms.front();
}

bool WormQueue::isEmpty() const
{
	//return std::all_of(wormQueue.cbegin(), wormQueue.cend(), [](const Team& team)
	//	{
	//		return team.worms.empty();
	//	});
	return wormQueue.empty();
}

int WormQueue::aliveTeams()
{
	return wormQueue.size();
}

void WormQueue::update(sf::Time deltaTime)
{
	for (auto& team : wormQueue)
	{
		for(auto& worm : team.worms)
			worm->update(deltaTime);
	}

}

void WormQueue::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// For now the most important thing is that
	// the current worm should be drawn last

	std::list<Team>::const_iterator team = currentTeam;
	++team;
	
	if (team == wormQueue.cend())
		team = wormQueue.cbegin();
	
	while (team->color != currentTeam->color)
	{

		for (auto beg = team->worms.crbegin(), end = team->worms.crend(); beg != end; ++beg)
			(*beg)->draw(target, states);

		++team;

		if (team == wormQueue.cend())
			team = wormQueue.cbegin();
	}

	for (auto beg = team->worms.crbegin(), end = team->worms.crend(); beg != end; ++beg)
		(*beg)->draw(target, states);
}

void WormQueue::handleEvents(const sf::Event& event)
{
	for (auto& team : wormQueue)
	{
		for (auto& worm : team.worms)
			worm->handleEvents(event);
	}
}

Worm& WormQueue::front()
{
	return *currentTeam->worms.front();
}

void WormQueue::removeDestroyed()
{
	for(auto team = wormQueue.begin(), end = wormQueue.end(); team != end; ++team)
	{
		// I start with removal of the worms from the teams
		auto marked_remove = std::remove_if(team->worms.begin(), team->worms.end(), std::mem_fn(&NodeScene::isDestroyed));
		team->worms.erase(marked_remove, team->worms.end());

		// Then If any team is empty I want to remove it from the list
		if (team->worms.empty())
		{
			if (*team == *currentTeam)
			{
				auto toDelete = currentTeam;
				++currentTeam;

				if (currentTeam == wormQueue.end())
					currentTeam = wormQueue.begin();
				
				team = wormQueue.erase(std::find(wormQueue.begin(), wormQueue.end(), *toDelete));
			}
			else
			{
				team = wormQueue.erase(std::find(wormQueue.begin(), wormQueue.end(), *team));
			}
			
			if(team == end)
				--team;

		}
	}
}
