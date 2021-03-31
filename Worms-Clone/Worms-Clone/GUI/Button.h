#ifndef BUTTON_H
#define BUTTON_H
#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../Resources/Resources.h"



#include "Component.h"

namespace GUI
{
    class Button : public Component
    {
    public:
        Button(const TextureManager& textures, const FontManager& fonts, sf::RenderWindow& window);
    	
        void onEnter() override;
        void onLeave() override;

        void activate() override;

        void setSize(int x, int y);

    	
        void setActiveFunction(std::function<void()> onActivate);
        void setText(const std::string& text);

        sf::FloatRect getGlobalBounds();

        void matchSizeToText(float padding = 0.f);
    	
    	
    	void handleEvents(const sf::Event& event) override;
    	void update() override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::function<void()> activateFunction;
        const sf::Texture& normalTexture;
        const sf::Texture& selectedTexture;

        sf::Sprite sprite;
        sf::Text text;

        sf::Vector2f size;

        sf::RenderWindow& window;
    };
}


#endif