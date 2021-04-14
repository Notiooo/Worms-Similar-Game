#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics/Rect.hpp>

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

/**
 * \brief Sets an object under another object
 * \tparam T1 Type of the first object
 * \tparam T2 Type of the second object
 * \param object Object to be set
 * \param reference_object The object relative to which the object is to be positioned
 * \param padding Additional distance to be maintained
 */
template <typename T1, typename T2>
void setPositionBelow(T1& object, const T2& reference_object, float padding=0.f)
{
	sf::Vector2f position = reference_object.getPosition();
	sf::Vector2f heightFromPosition = sf::Vector2f(0, reference_object.getGlobalBounds().height - reference_object.getLocalBounds().top);
	sf::Vector2f thisHeightFromPosition = sf::Vector2f(0, object.getGlobalBounds().height - object.getLocalBounds().top);
	
	object.setPosition(position + heightFromPosition + thisHeightFromPosition);
}

/**
 * \brief Calculates and finds the position just below another object
 * \tparam T 
 * \param reference_object The object whose positions should be calculated
 * \return Position under the passed object
 */
template <typename T>
sf::Vector2f getPositionBelow(const T& reference_object)
{
	sf::Vector2f position = reference_object.getPosition();
	sf::Vector2f heightFromPosition = sf::Vector2f(0, reference_object.getGlobalBounds().height - reference_object.getLocalBounds().top);
	return sf::Vector2f(position + heightFromPosition);
}

#endif