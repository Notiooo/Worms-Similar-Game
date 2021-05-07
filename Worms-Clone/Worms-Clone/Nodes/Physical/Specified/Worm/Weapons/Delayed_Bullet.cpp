#include "Delayed_Bullet.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "../../../CollideTypes.h"
#include "Hitbox.h"
#include "../../../NodePhysicalSpark.h"


Delayed_Bullet::Delayed_Bullet(b2World& world, const FontManager& fonts, sf::Vector2f position, sf::Texture& bulletTexture, const TextureManager& textures, float force, float range, sf::Time timeToDestroy):
	Bullet(world, position, bulletTexture, textures, force, range),
	timeToDestroy(timeToDestroy)
{
	counter.setFont(fonts.getResourceReference(Fonts_ID::ArialNarrow));
	counter.setFillColor(sf::Color::White);
	counter.setOutlineColor(sf::Color::Black);
	counter.setOutlineThickness(1.f);

	for (b2Fixture* fix = Body->GetFixtureList(); fix; fix = fix->GetNext())
	{
		delete reinterpret_cast<Collision*>(fix->GetUserData().pointer);
	}
	
	Body->DestroyFixture(fixture);

	// Defines its shape
	b2CircleShape Shape;
	Shape.m_radius = (sprite.getLocalBounds().width + sprite.getLocalBounds().height) / 2.f / 2.f / B2_SCALAR;
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.restitution = 0.4f;
	FixtureDef.shape = &Shape;
	fixture = Body->CreateFixture(&FixtureDef);

}

void Delayed_Bullet::updateThis(sf::Time deltaTime)
{
	NodePhysicalSprite::updateThis(deltaTime);
	const int timeLeft = static_cast<int>((timeToDestroy - clock.getElapsedTime()).asSeconds());
	counter.setString(std::to_string(timeLeft));
	if (timeLeft <= 0)
		setDestroyed();

	counter.setPosition(getAbsolutePosition());
}

void Delayed_Bullet::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	Bullet::drawThis(target, states);

	target.draw(counter);
}

void Delayed_Bullet::collision()
{
	// Do nothing
}
