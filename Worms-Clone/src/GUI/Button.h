#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "GUI/Component.h"
#include "Resources/Resources.h"

namespace GUI
{
	/**
     * \brief A button that can be pressed in two ways depending on the settings (right and left mouse button).
     */
    class Button : public Component
    {
    public:
        Button(const TextureManager& textures, const FontManager& fonts);


        /**
         * \brief Code that executes everytime when the player enters the button area.
         */
        void onEnter() override;

        /**
         * \brief Code that executes everytime when the player leaves the button area.
         */
        void onLeave() override;

        /**
         * \brief Function executed when a button is activated (pressed).
         */
        void activate() override;

        /**
         * \brief Function executed when a button is deactivated
         * (for example pressed with other button).
         */
        void deactivate() override;

        /**
         * \brief Sets a new button dimension
         * \param width Width of button
         * \param height Height of button
         */
        void setSize(int width, int height);

        /**
         * \brief Sets the function that is performed when the button is activated
         * \param onActivate Function to execute
         */
        void setActiveFunction(std::function<void(Button&)> onActivate);

        /**
         * \brief Sets the function that is performed when the button is deactivated
         * \param onDeactivate Function to execute
         */
        void setDeactiveFunction(std::function<void(Button&)> onDeactivate);
    	
        /**
         * \brief Sets the text inside the button
         * \param text Text to be placed inside the button
         */
        void setText(const std::string& text);

        /**
         * \brief Get the global bounding rectangle of the button
         * \return The global bounds of the button
         */
        sf::FloatRect getGlobalBounds() const override;

        /**
         * \brief Get the local bounding rectangle of the button
         * \return The local bounds of the button
         */
        sf::FloatRect getLocalBounds() const override;

        /**
         * \brief Resizes the button to fit the text it contains
         * \param padding Additional distance of the button edge from the text
         */
        void matchSizeToText(float padding = 0.f);

        /**
    	 * \brief Sets button under another object
    	 * \tparam T Type of object on the basis of which we set the button
    	 * \param object The object on the basis of which we set the button
    	 * \param padding Additional distance between reference point and button
    	 */
    	template <typename T>
        void setPositionBelow(const T& object, float padding = 0.f);

        /**
         * \brief It takes input (event) from the user and interprets it
         * \param event user input
         */
    	void handleEvents(const sf::Event& event) override;

        /**
         * \brief Updates the status/logic of the button
         * \param mousePosition Current mouse position
         */
    	void update(sf::Vector2f mousePosition) override;

        /**
		 * \brief Draws only this state to the passed target
		 * \param target where it should be drawn to
		 * \param states provides information about rendering process (transform, shader, blend mode)
		 */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
    	// === Graphical part of button == //
        const sf::Texture& normalTexture; //!< Texture that displays when the button is not selected
        const sf::Texture& selectedTexture; //!< Texture that is displayed when the user hovers the cursor over a button
        sf::Sprite sprite; //!< Sprite displaying the current button texture
        sf::Text text; //!< Text inside the button
        sf::Vector2f size; //!< Dimensions of the button


        // === Functional part of button == //
        std::function<void(Button&)> activateFunction; //!< A function that is performed when a button is pressed (activated)
        std::function<void(Button&)> deactivateFunction; //!< A function that is performed when a button is pressed with alternative key (deactivated)
    };

	/**
     * \brief Function to place a button under another object
     * \tparam T Other object type
     * \param object The object under which the button should be placed
     * \param padding Additional spacing that the button should maintain
     */
    template <typename T>
    void Button::setPositionBelow(const T& object, float padding)
    {
    	const sf::Vector2f pos(object.getPosition().x, object.getPosition().y);
    	
        auto offset = sf::Vector2f(0, object.getGlobalBounds().height);

        padding *= getScale().y;
        offset *= getScale().y;
        setPosition(pos + offset + sf::Vector2f(0, padding));
    }

}


#endif