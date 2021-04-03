#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics/Rect.hpp>
#include <iostream>

/**
 * \brief Sets the origin to the centre of the drawing object.
 * \tparam T Drawable object type
 * \param drawable_obj Object to centre
 */
template <typename T>
void centerOrigin(T& drawable_obj)
{
	const sf::FloatRect rect = drawable_obj.getLocalBounds();
	drawable_obj.setOrigin(rect.left + rect.width / 2.f,
		rect.top + rect.height / 2.f);
}

template <typename T1, typename T2>
void setPositionBelow(T1& object, const T2& reference_object, float padding=0.f)
{
	sf::Vector2f position = reference_object.getPosition();
	sf::Vector2f heightFromPosition = sf::Vector2f(0, reference_object.getGlobalBounds().height);
	sf::Vector2f thisHeightFromPosition = sf::Vector2f(0, object.getGlobalBounds().height);
	
	object.setPosition(position + heightFromPosition + thisHeightFromPosition);
}

template <typename T>
sf::Vector2f getPositionBelow(const T& reference_object)
{
	sf::Vector2f position = reference_object.getPosition();
	sf::Vector2f heightFromPosition = sf::Vector2f(0, reference_object.getGlobalBounds().height);
	return sf::Vector2f(position + heightFromPosition);
}

#endif