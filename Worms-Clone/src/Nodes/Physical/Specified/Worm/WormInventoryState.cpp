#include "pch.h"
#include "Nodes/Physical/Specified/Worm/WormInventoryState.h"

#include <SFML/Graphics/RenderTarget.hpp>

WormInventoryState::WormInventoryState(StateStack& stack, Worm& worm, TextureManager& textures, const FontManager& fonts, sf::RenderWindow& window) :
	State(stack),
	window(window),
	worm(worm),
	menuSprite(textures.getResourceReference(Textures_ID::Inventory)),
	ropeSprite(textures.getResourceReference(Textures_ID::Rope)),
	stopPosition(worm.getAbsolutePosition().y),
	font(fonts.getResourceReference(Fonts_ID::ArialNarrow))
{
	#ifdef SHOW_WORM_STATES
		worm.setName("InventoryState");
	#endif
	
	// Set origin to center-bottom
	menuSprite.setOrigin(menuSprite.getLocalBounds().width / 2.f, menuSprite.getLocalBounds().height);
	menuSprite.setPosition(worm.getAbsolutePosition().x + menuSprite.getLocalBounds().width, (window.mapPixelToCoords({0, 0}).y - menuSprite.getLocalBounds().height));

	// Set origin to the bottom of the rope
	ropeSprite.setTextureRect(sf::IntRect(0, 0, ropeSprite.getTextureRect().width, 1000));
	ropeSprite.setOrigin(ropeSprite.getLocalBounds().width / 2.f, ropeSprite.getLocalBounds().height);
	ropeSprite.setPosition(menuSprite.getPosition().x, menuSprite.getPosition().y);

	// Give special selection color to the current selected weapon
	worm.selectedWeapon->second->getThumbnailSprite().setColor(selectedColor);


	// Prepare proper text to display amount of left bullets
	amounts.reserve(worm.inventory.size());
	for (auto& [numberOfUses, weapon] : worm.inventory)
	{
		sf::Text amountOfBullets;
		amountOfBullets.setFont(font);
		amountOfBullets.setString(std::to_string(numberOfUses));
		amountOfBullets.setOutlineColor(sf::Color::Black);
		amountOfBullets.setOutlineThickness(1.f);
		amountOfBullets.setCharacterSize(18);
		amountOfBullets.setOrigin(amountOfBullets.getLocalBounds().width, amountOfBullets.getLocalBounds().height);
		amounts.push_back(amountOfBullets);
	}

}

void WormInventoryState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Both object are drawn independent of transform
	target.draw(ropeSprite);
	target.draw(menuSprite);
	
	for(const auto& [numberOfUses, weapon] : worm.inventory)
		target.draw(weapon->getThumbnailSprite());

	for (const auto& bulletsLeft : amounts)
		target.draw(bulletsLeft);
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


	// Variables used to proper placement of items
	const auto globalBounds = menuSprite.getGlobalBounds();
	auto rowSpacing = 0.f;
	auto columnSpacing = 0.f;

	// Display weapon thumbnails

	// The bullets are iterated separately
	auto bulletsLeft = amounts.begin();
	for (auto& [numberOfUses, weapon] : worm.inventory)
	{
		// Set position of thumbnail and the bullets
		auto& thumbnail = weapon->getThumbnailSprite();

		thumbnail.setPosition(globalBounds.left + padding + columnSpacing,
								globalBounds.top + padding + rowSpacing);
		
		bulletsLeft->setPosition(thumbnail.getPosition() + 
			sf::Vector2f(thumbnail.getLocalBounds().width, thumbnail.getLocalBounds().height));
		++bulletsLeft;

		// Move further to the next column
		columnSpacing += spacing + thumbnail.getLocalBounds().width;

		// Check if it should go to next row
		if(columnSpacing + thumbnail.getLocalBounds().width > menuSprite.getLocalBounds().width)
		{
			// Move back to the first column, and start displaying in next row
			columnSpacing = 0;
			rowSpacing += thumbnail.getLocalBounds().height + spacing;
		}
	}
	return false;
}

bool WormInventoryState::handleEvent(const sf::Event& event)
{
	// If player choose different weapon from the equipment
	// then it should change the current weapon to new one
	// and display the chosen weapon in different way in equipment
	if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
	{
		const auto position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		if(menuSprite.getGlobalBounds().contains(position))
		{
			for (auto& weapon : worm.inventory)
			{
				auto& thumbnail = weapon.second->getThumbnailSprite();

				// Check if this weapon contains in coordinations where
				// the player has clicked
				if (thumbnail.getGlobalBounds().contains((position)))
				{
					if (&weapon != worm.selectedWeapon)
					{
						// The current weapon should have again normal color
						worm.selectedWeapon->second->getThumbnailSprite().setColor(sf::Color::White);
						worm.selectedWeapon = &weapon;

						// The selected weapon should have different color
						thumbnail.setColor(selectedColor);
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
