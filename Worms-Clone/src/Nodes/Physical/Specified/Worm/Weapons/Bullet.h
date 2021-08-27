#ifndef BULLET_H
#define BULLET_H

#include "Nodes/Physical/NodePhysicalSprite.h"
#include "Resources/Resources.h"
#include "Sounds/SoundPlayer.h"


/**
 * \brief This is a standard button that, when it collides into place,
 * creates a Hitbox class object that checks the worms in its range and deals damage to them.
 */
struct Bullet : public NodePhysicalSprite
{
	Bullet(b2World& world, SoundPlayer& sounds, sf::Vector2f position, sf::Texture& bulletTexture, const TextureManager& textures, float force, float range);

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
	void setSparkColor(const sf::Color& color) noexcept;

protected:
	/**
	 * \brief A function that checks if an object is marked as ready for deletion.
	 * Additionally, it applies visual effects where the object is removable.
	 * \return True if it is ready to be removed, false otherwise.
	 */
	bool isDestroyed() override;
	
	float force; //!< The force with which the bullet will strike (the power of the damage inflicted).
	float range; //!< The range that the bullet will cover after the explosion

private:
	bool collided = false; //!< A flag that checks if there has been a collision between a bullet and another physical object.
	sf::Color sparkColor = sf::Color::White; //!< Color of the particles that will disperse when the projectile explodes.
	const sf::Texture& smokeTexture; //!< Texture left after the falling sparks after the explosion
	
	SoundPlayer& soundPlayer; //!< A player that allows to play sounds in the game world

};

#endif // !BAZOOKA_H
