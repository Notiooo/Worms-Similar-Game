#ifndef COMPONENT_H
#define COMPONENT_H
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

namespace GUI
{
	/**
	 * \brief A GUI object that the player can interact with.
	 */
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		Component();
		~Component() override = default;

		/**
		 * \brief Checks whether the user moved the mouse over the component
		 * \return True if the user hovers the mouse over the object, false otherwise
		 */
		bool isSelected() const;
		
		/**
		 * \brief Code that executes everytime when the player enters the component area.
		 */
		virtual void onEnter();

		
		/**
		 * \brief Code that executes everytime when the player leaves the component area.
		 */
		virtual void onLeave();

		/**
		 * \brief Checks whether a component has been pressed/activated.
		 * \return True, if activated, false otherwise
		 */
		virtual bool isActive() const;

		/**
		 * \brief Function executed when a component is activated (pressed).
		 */
		virtual void activate();

		/**
		 * \brief Function executed when a component is deactivated
		 * (for example pressed with other button).
		 */
		virtual void deactivate();

		/**
		 * \brief Get the global bounding rectangle of the button
		 * \return The global bounds of the button
		 */
		virtual sf::FloatRect getGlobalBounds() const = 0;

		/**
		 * \brief Get the local bounding rectangle of the button
		 * \return The local bounds of the button
		 */
		virtual sf::FloatRect getLocalBounds() const = 0;

		/**
		 * \brief Sets position of a component under another component
		 * \param object Another component against which we set the new position
		 * \param padding Additional distance between components
		 */
		virtual void setPositionBelow(const Component& object, float padding = 0.f);
		
		/**
		 * \brief It takes input (event) from the user and interprets it
		 * \param event user input
		 */
		virtual void handleEvents(const sf::Event& event) = 0;

		/**
		 * \brief Updates the status/logic of the component
		 * \param mousePosition Current mouse position
		 */
		virtual void update(sf::Vector2f mousePosition) = 0;


	private:
		bool _isSelected; //!< Flag telling if the component is currently selected
		bool _isActive; //!< Flag telling if the component is currently activated
	};
	
}


#endif