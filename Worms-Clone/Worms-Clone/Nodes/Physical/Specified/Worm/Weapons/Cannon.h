#ifndef CANNON_H
#define CANNON_H

#include "Weapon.h"

#include "../../../../../Resources/Resources.h"

/**
 * \brief A weapon with a short range of fire but high damage.
 */
class Cannon : public Weapon
{
public:
	Cannon(b2World& world, TextureManager& textures);

	/**
	 * \brief Is the cannon an activated weapon or a loaded weapon (via the shooting bar).
	 * \return True if it is activated weapon, false if it need to be loaded
	 */
	bool isActivation() override;


	/**
	 * \brief Does using the Cannon end the round
	 * \return True usage of cannon ends the round, false otherwise
	 */
	bool isRoundEnding() override;
};

#endif // !CANNON_H