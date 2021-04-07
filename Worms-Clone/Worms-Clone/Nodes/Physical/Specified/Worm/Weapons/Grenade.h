#ifndef GRENADE_H
#define GRENADE_H

#include "SFML/Window/Event.hpp"
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
	
	bool isActivation() override;
	bool isRoundEnding() override;

private:
	const FontManager& fonts;
};

#endif // !BAZOOKA_H
