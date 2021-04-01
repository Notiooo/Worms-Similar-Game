#ifndef INCREMENTALBUTTON_H
#define INCREMENTALBUTTON_H
#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../Resources/Resources.h"



#include "Button.h"

namespace GUI
{
    class IncrementalButton : public Button
    {
    public:
        IncrementalButton(const TextureManager& textures, const FontManager& fonts, sf::RenderWindow& window);

        void activate() override;
        void deactivate() override;

    private:

    	
    	// Hide those functions
        using Button::setActiveFunction;
        using Button::setDeactiveFunction;

    };
}


#endif