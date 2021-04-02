#include "WormQueue.h"

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
	while (currentTeam->worms.empty())
	{
		++currentTeam;
		if (currentTeam == wormQueue.end())
			currentTeam = wormQueue.begin();
	}
	
	std::unique_ptr<Worm> wormPtr = std::move(currentTeam->worms.front());
	currentTeam->worms.pop_front();
	currentTeam->worms.push_back(std::move(wormPtr));

	++currentTeam;
	if (currentTeam == wormQueue.end())
		currentTeam = wormQueue.begin();
	return *currentTeam->worms.front().get();
}

Worm& WormQueue::getPreviousWorm()
{
	std::unique_ptr<Worm> wormPtr = std::move(currentTeam->worms.back());
	currentTeam->worms.pop_back();

	Worm& worm = *wormPtr;

	currentTeam->worms.push_front(std::move(wormPtr));

	--currentTeam;
	if (currentTeam == wormQueue.begin())
		currentTeam = wormQueue.end();
	return worm;
}

bool WormQueue::isEmpty() const
{
	return std::all_of(wormQueue.cbegin(), wormQueue.cend(), [](const Team& team)
		{
			return team.worms.empty();
		});
}

int WormQueue::aliveTeams()
{
	int counter = 0;
	for(auto& team : wormQueue)
	{
		if (!team.worms.empty())
			++counter;
	}
	return counter;
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

	for (auto& worm : team->worms)
		worm->draw(target, states);
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
	while (currentTeam->worms.empty())
	{
		++currentTeam;
		if (currentTeam == wormQueue.end())
			currentTeam = wormQueue.begin();
	}
	return *currentTeam->worms.front();
}

void WormQueue::removeDestroyed()
{
	for(Team& team : wormQueue)
	{
		auto marked_remove = std::remove_if(team.worms.begin(), team.worms.end(), std::mem_fn(&NodeScene::isDestroyed));
		team.worms.erase(marked_remove, team.worms.end());
	}
}
