#ifndef WEAPON_H
#define WEAPON_H

#include "SFML/Graphics/Sprite.hpp"

#pragma warning(push, 0)
#include "box2d/box2d.h"
#pragma warning(pop)

#include "../../../../NodeScene.h"
#include "../../../../../Resources/Resources.h"
#include "../../../../Particles/NodeParticle.h"

class Worm;

/**
 * \brief An abstract class of weapon that can be activated or
 * fired with an earlier shooting bar charge.
 */
class Weapon : public NodeScene
{
public:
	Weapon(b2World& world, sf::Texture& weapon, sf::Texture& thumbnail, sf::Texture& bullet, const TextureManager& textures);

	/**
	 * \brief The firing function of a weapon that will be executed when the shooting bar is loaded.
	 * \param rootNode A node that will serve as the parent of a bullet created and added to the game world.
	 * \param position The position at which the bullet will form.
	 * \param force The force with which the bullet will be fired
	 */
	virtual void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force);

	/**
	 * \brief Activates the weapon. Used when a weapon does not shoot with usage of shooting bar but can be activated instantly.
	 * \param worm A worm that uses a particular weapon.
	 */
	virtual void activation(Worm& worm);

	// === Setters === //
	/**
	 * \brief Sets the maximum damage this weapon is capable of dealing.
	 * \param dmg Maximum possible damage
	 */
	void setMaxDmg(float dmg);

	
	/**
	 * \brief Sets the maximum distance from the explosion point
	 * from which it deals damage
	 * \param rng Maximum possible range at which it still deals damage
	 */
	void setRange(float rng);

	
	/**
	 * \brief Sets the colour of the particles present after the explosion
	 * \param color Colour of the particles
	 */
	void setSparkColor(const sf::Color& color);

	/**
	 * \brief Check if weapon should have shooting bar, or it should just activate with a button
	 * \return True if it is activated with a button, False if it should have been loaded with shooting bar
	 */
	virtual bool isActivation() = 0;

	
	/**
	 * \brief Check if usage of the weapon should end the round
	 * \return True usage ends the round, false otherwise
	 */
	virtual bool isRoundEnding() = 0;

	/**
	 * \brief Returns a sprite which is a graphical representation of the weapon thumbnail in the inventory
	 * \return A thumbnail that is a graphical representation of a weapon in an inventory.
	 */
	sf::Sprite& getThumbnailSprite();
	
	/**
	 * \brief Adjusts the weapon to a given angle
	 * \param angle Angle at which the weapon should be positioned
	 */
	void rotateWeapon(float angle);

	/**
	 * \brief Draws only this Weapon to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief Updates the logic of the weapon.
	 * \param deltaTime the time that has passed since the game was last updated
	 */
	void updateThis(sf::Time deltaTime) override;

protected:
	// === Graphical === //
	sf::Sprite weaponSprite; //!< Sprite which is a visual representation of the weapon in the game
	sf::Sprite thumbnailSprite; //!< Sprite which is a visual representation of the weapon in the inventory
	sf::Texture& bulletTexture; //!< Sprite which is a visual representation of the bullet in the game
	sf::Color bulletSparksColor = sf::Color::White; //!< Colour of the explosion after the projectile explosion
	const TextureManager& textures;
	

	// === Parameters === //
	float attackDmg = 0.f; //!< Maximum damage this weapon can deal
	float range = 0.f; //!< Weapon striking range
	
	b2World& physicalWorld; //!< Physical simulation of the game world
};

#endif // !WEAPON_H
