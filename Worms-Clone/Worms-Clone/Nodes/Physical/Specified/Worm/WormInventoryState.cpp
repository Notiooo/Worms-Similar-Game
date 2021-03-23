#include "WormInventoryState.h"

#include <SFML/Graphics/RenderTarget.hpp>

WormInventoryState::WormInventoryState(StateStack& stack, Worm& worm, TextureManager& textures, sf::RenderWindow& window) :
	State(stack),
	window(window),
	worm(worm),
	menuSprite(textures.getResourceReference(Textures_ID::Inventory)),
	ropeSprite(textures.getResourceReference(Textures_ID::Rope)),
	stopPosition(worm.getAbsolutePosition().y)
{
	// Set origin to center-bottom
	menuSprite.setOrigin(menuSprite.getLocalBounds().width / 2.f, menuSprite.getLocalBounds().height);
	menuSprite.setPosition(worm.getAbsolutePosition().x + menuSprite.getLocalBounds().width, (window.mapPixelToCoords({0, 0}).y - menuSprite.getLocalBounds().height));
	// Set origin to the bottom of the rope
	ropeSprite.setTextureRect(sf::IntRect(0, 0, ropeSprite.getTextureRect().width, 1000));
	ropeSprite.setOrigin(ropeSprite.getLocalBounds().width / 2.f, ropeSprite.getLocalBounds().height);
	ropeSprite.setPosition(menuSprite.getPosition().x, menuSprite.getPosition().y);


	// Initially set opacity of selected weapon to 30
	worm.selectedWeapon->second->getThumnbailSprite().setColor(sf::Color(255, 255, 255, 30));


}

void WormInventoryState::draw() const
{
}

void WormInventoryState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ropeSprite);
	target.draw(menuSprite);
	for(auto& weapon : worm.inventory)
	{
		target.draw(weapon.second->getThumnbailSprite());
	}
}

bool WormInventoryState::update(sf::Time deltaTime)
{

	// Animation of the inventory coming from above
	if (!stopAnimating)
	{
		if (menuSprite.getPosition().y < stopPosition)
		{
			menuSprite.move(0, moveSpeed * deltaTime.asSeconds());
			ropeSprite.move(0, moveSpeed * deltaTime.asSeconds());
		}
		else
			stopAnimating = true;
	}

	const auto globalBounds = menuSprite.getGlobalBounds();
	float rowSpacing = 0;
	float columnSpacing = 0;
	for (auto& weapon : worm.inventory)
	{
		auto& thumbnail = weapon.second->getThumnbailSprite();

		thumbnail.setPosition(globalBounds.left + padding + rowSpacing, globalBounds.top + padding + columnSpacing);
		rowSpacing += spacing + thumbnail.getLocalBounds().width;
		if(rowSpacing + thumbnail.getLocalBounds().width > menuSprite.getLocalBounds().width)
		{
			rowSpacing = 0;
			columnSpacing += thumbnail.getLocalBounds().height + spacing;
		}
	}
	return false;
}

bool WormInventoryState::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
	{
		sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		if(menuSprite.getGlobalBounds().contains(position))
		{
			for (auto& weapon : worm.inventory)
			{
				auto& thumbnail = weapon.second->getThumnbailSprite();
				if (thumbnail.getGlobalBounds().contains((position)))
				{
					if (&weapon != worm.selectedWeapon)
					{
						worm.selectedWeapon->second->getThumnbailSprite().setColor(sf::Color(255, 255, 255, 255));
						worm.selectedWeapon = &weapon;
						thumbnail.setColor(sf::Color(255, 255, 255, 30));
					}
				}
			}
		}
		else
		{
			worm.activateState(State_ID::WormPlayState);
		}
	}
	return false;
}
