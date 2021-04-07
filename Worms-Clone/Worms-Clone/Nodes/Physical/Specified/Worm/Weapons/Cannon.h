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

	bool isActivation() override;
	bool isRoundEnding() override;
};

#endif // !BAZOOKA_H
