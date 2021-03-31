#ifndef COMPONENT_H
#define COMPONENT_H
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

namespace GUI
{
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		Component();
		virtual ~Component();

		bool isSelected() const;
		virtual void onEnter();
		virtual void onLeave();

		virtual bool isActive() const;
		virtual void activate();
		virtual void deactivate();

		virtual void handleEvents(const sf::Event& event) = 0;
		virtual void update() = 0;

	private:
		bool _isSelected;
		bool _isActive;
	};
	
}


#endif