#ifndef WEAPON_H
#define WEAPON_H

#include "../Worm.h"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "box2d/box2d.h"
#include "../../../../NodeScene.h"
#include "../../../../../Resources/Resources.h"

class Weapon : public NodeScene
{
public:
	Weapon(b2World& world, sf::Texture& weapon, sf::Texture& thumbnail, sf::Texture& bullet);

	virtual void shoot(NodeScene* rootNode, sf::Vector2f position, sf::Vector2f force);

	virtual void activation(Worm& worm);

	// === Setters === //
	/**
	 * \brief Sets the maximum damage this weapon is capable of dealing.
	 * \param dmg Maximum possible damage
	 */
	void setMaxDmg(float dmg);

	
	/**
	 * \brief Sets the maximum distance from the explosion point
	 * from which it deals damage
	 * \param rng Maximum possible range at which it still deals damage
	 */
	void setRange(float rng);

	
	/**
	 * \brief Sets the colour of the particles present after the explosion
	 * \param color Colour of the particles
	 */
	void setSparkColor(const sf::Color& color);

	/**
	 * \brief Check if weapon should have shooting bar, or it should just activate with a button
	 * \return True if it is activated with a button, False if it should have been loaded with shooting bar
	 */
	virtual bool isActivation() = 0;

	
	/**
	 * \brief Check if usage of the weapon should end the round
	 * \return True usage ends the round, false otherwise
	 */
	virtual bool isRoundEnding() = 0;

	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateThis(sf::Time deltaTime) override;

	void rotateWeapon(float angle);
	sf::Sprite& getThumbnailSprite();


protected:
	// === Graphical === //
	sf::Sprite weaponSprite;
	sf::Sprite thumbnailSprite;
	sf::Texture& bulletTexture;
	sf::Color bulletSparksColor = sf::Color::White;


	// === Parameters === //
	float attackDmg = 0.f;
	float range = 0.f;
	
	b2World& physicalWorld;
};

#endif // !WEAPON_H
