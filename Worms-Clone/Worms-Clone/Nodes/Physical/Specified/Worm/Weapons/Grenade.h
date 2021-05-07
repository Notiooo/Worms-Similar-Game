#ifndef GRENADE_H
#define GRENADE_H

#include "SFML/Graphics/Sprite.hpp"
#include "Weapon.h"
#include "Bullet.h"

#include "../../../../../Resources/Resources.h"

/**
 * \brief A grenade is a weapon that explodes after a specified.
 * Deals high damage and has a long range of effect
 */
class Grenade : public Weapon
{
public:
	Grenade(b2World& world, TextureManager& textures, const FontManager& fonts);

	/**
	 * \brief The firing function of a weapon that will be executed when the shooting bar is loaded.
	 * \param rootNode A node that will serve as the parent of a bullet created and added to the game world.
	 * \param position The position at which the bullet will form.
	 * \param force The force with which the bullet will be fired
	 */
	void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force) override;
	
	/**
	 * \brief Is the grenade an activated weapon or a loaded weapon (via the shooting bar).
	 * \return True if it is activated weapon, false if it need to be loaded
	 */
	bool isActivation() const noexcept override;


	/**
	 * \brief Does using the Grenade end the round
	 * \return True usage of grenade ends the round, false otherwise
	 */
	bool isRoundEnding() const noexcept override;

private:
	const FontManager& fonts; //!< Needed to load a font displaying the time to explosion
};

#endif // !GRENADE_H
