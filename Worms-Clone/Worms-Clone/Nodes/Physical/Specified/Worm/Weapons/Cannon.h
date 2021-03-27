#ifndef CANNON_H
#define CANNON_H

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "Weapon.h"
#include "Bullet.h"

#include "../../../../../Resources/Resources.h"

class Cannon : public Weapon
{
public:
	Cannon(b2World& world, TextureManager& textures);
};

#endif // !BAZOOKA_H
