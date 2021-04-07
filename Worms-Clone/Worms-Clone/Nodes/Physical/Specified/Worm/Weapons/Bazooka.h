#ifndef BAZOOKA_H
#define BAZOOKA_H

#include "Weapon.h"
#include "Bullet.h"

#include "../../../../../Resources/Resources.h"

class Bazooka : public Weapon
{
public:
	Bazooka(b2World& world, TextureManager& textures);

	bool isActivation() override;
	bool isRoundEnding() override;
};

#endif // !BAZOOKA_H
