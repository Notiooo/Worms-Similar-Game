#ifndef WORMPLAYSTATE_H
#define WORMPLAYSTATE_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "Nodes/Physical/Specified/Worm/Worm.h"
#include "Nodes/Physical/Specified/Worm/WormMoveableState.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Weapon.h"
#include "States/State.h"

/**
 * \brief A worm state in which the worm is controlled by the player.
 *
 * This state allows the worm to move around the game world.
 * Change states (for example, open equipment) or shoot.
 */
class WormPlayState : public WormMoveableState
{
public:
	WormPlayState(StateStack& stack, Worm& worm, const TextureManager& textures, SoundPlayer& sounds);

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

	/**
	 * \brief A function that shoots a bullet
	 *
	 * Depending on the weapon setting,
	 * it can change the worm's state to "HideState".
	 */
	void shoot();

private:
	/**
	 * \brief It interprets player input and operates the shooting system according to it.
	 * \param event input of the player
	 */
	void handleShooting(const sf::Event& event);
	
	/**
	 * \brief Updates all things in terms of the shooting system
	 * \param deltaTime Time elapsed since the previous frame
	 */
	void updateShooting(sf::Time deltaTime);

	SoundPlayer& soundPlayer; //!< A player that allows to play sounds in the game world

	//// ====== SHOOTING ====== ////

	// === Controls === /
	sf::Keyboard::Key shootingKey = sf::Keyboard::Enter; //!< Button with which the player starts shooting
	sf::Keyboard::Key pointHigherKey = sf::Keyboard::Left; //!< Button that raises the pointer upwards
	sf::Keyboard::Key pointLowerKey = sf::Keyboard::Right; //!< Button that raises the pointer downwards

	// === Pointer of shooting === //

	sf::CircleShape triangularPointer; //!< Graphical class to be drawn representing the pointer
	sf::Vector2f pointer; //!< Position of the pointer
	float pointerLength = 100.f; //!< How far away from the worm is the pointer
	float pointerSpeed = 2.f; //!< How fast the pointer moves/rotates
	float pointerAngle = b2_pi / 2.f; //!< Default angle at which the pointer points

	// === The Shooting Bar === //

	float maxShootingForce = 35000.f; //!< The maximum power with which a worm can shoot -- this translates into shooting distance.
	float currentShootingForce = 0.f; //!< Current firing power used when player is shooting
	float shootingLoadingSpeed = (maxShootingForce * 1/2); //!< How fast the bar is loading per second.
	float shootingBarSize = 80.f; //!< Drawing length of the shooting bar
	sf::RectangleShape shootingBar; //!< Graphical class to be drawn representing the shooting bar
};
#endif