#ifndef DELAYED_BULLET_H
#define DELAYED_BULLET_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

#include "SFML/Graphics/Sprite.hpp"
#include "../../../../../Resources/Resources.h"
#include "Bullet.h"


struct Delayed_Bullet : public Bullet
{
	Delayed_Bullet(b2World& world, FontManager& fonts, sf::Vector2f position, sf::Texture& texture, float force, float range, sf::Time timeToDestroy);

	void updateThis(sf::Time deltaTime) override;
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	void collision() override;


private:
	sf::Time timeToDestroy = sf::Time::Zero; //!< If time != 0 is set, then it will explode after given amount of time
	sf::Clock clock; //!< Clock that measures time to explosion

	sf::Text counter; //!< Text displaying the time left to explosion
	

};

#endif // !BAZOOKA_H
