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
		~Component() override;

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

		virtual void handleEvents(const sf::Event& event) = 0;
		virtual void update() = 0;

	private:
		bool _isSelected; //!< Flag telling if the component is currently selected
		bool _isActive; //!< Flag telling if the component is currently activated
	};
	
}


#endif