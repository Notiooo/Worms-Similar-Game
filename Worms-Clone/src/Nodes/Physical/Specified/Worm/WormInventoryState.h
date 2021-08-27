#ifndef WORMINVENTORYSTATE_H
#define WORMINVENTORYSTATE_H

#include "Nodes/Physical/Specified/Worm/Worm.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Weapon.h"
#include "States/State.h"

/**
 * \brief A condition that displays the player's inventory and allows
 * them to change to another weapon.
 *
 * It also displays the number of weapons the player has.
 */
class WormInventoryState : public State
{
public:
	WormInventoryState(StateStack&, Worm&, TextureManager& textures, const FontManager& fonts, sf::RenderWindow& window);


	/**
	 * \brief Draws only this state (current state of the worm) to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the status of the worm
	 * \param deltaTime the time that has passed since the last frame.
	 */
	bool update(sf::Time deltaTime) override;

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	bool handleEvent(const sf::Event& event) override;

private:

	sf::RenderWindow& window;
	const sf::Font& font;
	Worm& worm; //!< Worm for which inventory should it display

	// === Inventory === //
	sf::Sprite ropeSprite; //!< Rope at which the menu is swinging
	sf::Sprite menuSprite; //!< The background of the menu
	sf::Color selectedColor = sf::Color::Red; //!< Color of selected weapon
	std::vector<sf::Text> amounts; //!< Contains text indicating the number of available uses for a given weapon.

	// === Animating the menu === //
	float moveSpeed = 800.f; // how fast it moves down
	float stopPosition; // Y at which it stops
	bool stopAnimating = false;

	// === Additional variables for setting the display of weapons === //
	float padding = 10.f; //!< Additional distance from the upper left side of the equipment
	float spacing = 10.f; //!< Distance between weapon columns
	
};
#endif