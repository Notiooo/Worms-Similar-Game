#ifndef BAZOOKA_H
#define BAZOOKA_H

#include "Weapon.h"
#include "Bullet.h"

#include "../../../../../Resources/Resources.h"

/**
 * \brief The bazooka is a long-range weapon with medium damage.
 * Its projectiles explode on contact with a physical object.
 */
class Bazooka final : public Weapon
{
public:
	Bazooka(b2World& world, TextureManager& textures, SoundPlayer& soundPlayer);

	/**
	 * \brief Is the bazooka an activated weapon or a loaded weapon (via the shooting bar).
	 * \return True if it is activated weapon, false if it need to be loaded
	 */
	bool isActivation() const noexcept override;

	
	/**
	 * \brief Does using the Bazooka end the round
	 * \return True usage of bazooka ends the round, false otherwise
	 */
	bool isRoundEnding() const noexcept override;
};

#endif // !BAZOOKA_H
