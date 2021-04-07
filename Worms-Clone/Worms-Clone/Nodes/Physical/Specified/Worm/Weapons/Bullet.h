#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics/Sprite.hpp"
#include "../../../../Physical/NodePhysicalSprite.h"


/**
 * \brief This is a standard button that, when it collides into place,
 * creates a Hitbox class object that checks the worms in its range and deals damage to them.
 */
struct Bullet : public NodePhysicalSprite
{
	Bullet(b2World& world, sf::Vector2f position, sf::Texture& texture, float force, float range);

	void updateThis(sf::Time deltaTime) override;

	/**
	 * \brief A function that executes at the moment of explosion (immediately after the collision).
	 */
	void explode();

	/**
	 * \brief A function that executes upon collision with a physical object.
	 */
	virtual void collision();

	/**
	 * \brief Sets the color of the particles that will disperse when the projectile explodes.
	 * \param color Color of the particles
	 */
	void setSparkColor(const sf::Color& color);

	/**
	 * \brief A function that checks if an object is marked as ready for deletion.
	 * \return True if it is ready to be removed, false otherwise.
	 */
	bool isDestroyed() override;

protected:
	float force; //!< The force with which the bullet will strike (the power of the damage inflicted).
	float range; //!< The range that the bullet will cover after the explosion

private:
	bool collided = false; //!< A flag that checks if there has been a collision between a bullet and another physical object.
	sf::Color sparkColor = sf::Color::White; //!< Color of the particles that will disperse when the projectile explodes.

};

#endif // !BAZOOKA_H
