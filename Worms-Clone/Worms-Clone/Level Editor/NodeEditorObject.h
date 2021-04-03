#ifndef NODEEDITOROBJECT_H
#define NODEEDITOROBJECT_H

#include <SFML/Graphics/Text.hpp>

#include "../Nodes/NodeScene.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../Resources/Resources.h"

class NodeEditorObject : public sf::Drawable, public sf::Transformable
{
public:
	NodeEditorObject(const TextureManager& textures, const FontManager& fonts);

	sf::FloatRect getGlobalBounds() const;
	void updateMouse(const sf::Vector2f& mousePosition);

	void update(sf::Time deltaTime);

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	void handleEvent(const sf::Event event);

	bool isSelected();

	void select();

	void unselect();

	bool isActivated();
	
	void activate();

	void deactivate();

	void setName(const std::string& name);
	void setSize(float width, float height);
	sf::Vector2f getSize();

private:

	sf::Sprite nodeSprite;
	sf::Text nodeName;

	bool _isSelected = false;
	bool _isActivated = false;

	float movingSpeed = 100.f;
	float rotationSpeed = 50.f;
	float sizeChangeSpeed = 100.f;

};

#endif