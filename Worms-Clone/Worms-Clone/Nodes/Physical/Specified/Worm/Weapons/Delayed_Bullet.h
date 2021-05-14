#ifndef DELAYED_BULLET_H
#define DELAYED_BULLET_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

#include "SFML/Graphics/Sprite.hpp"
#include "../../../../../Resources/Resources.h"
#include "Bullet.h"


/**
 * \brief A bullet that does not explode on impact but after a sufficient amount of time has passed.
 */
struct Delayed_Bullet : public Bullet
{
	Delayed_Bullet(b2World& world, SoundPlayer& sounds, const FontManager& fonts, sf::Vector2f position, sf::Texture& bulletTexture, const TextureManager& textures, float force, float range, sf::Time timeToDestroy);

	/**
	 * \brief A function that updates the projectile status and checks
	 * if enough time has passed for the bullet to explode.
	 * \param deltaTime Time passed since the last frame
	 */
	void updateThis(sf::Time deltaTime) override;

	/**
	 * \brief Draws only this Bullet to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * \brief An empty function specifying that nothing happens at the time of the collision.
	 */
	void collision() override;


private:
	sf::Time timeToDestroy = sf::Time::Zero; //!< If time != 0 is set, then it will explode after given amount of time
	sf::Clock clock; //!< Clock that measures time to explosion

	sf::Text counter; //!< Text displaying the time left to explosion

	SoundPlayer& soundPlayer;
	

};

#endif // !DELAYED_BULLET_H
