#include "NodeEditorObject.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "../utils.h"

NodeEditorObject::NodeEditorObject(const TextureManager& textures, const FontManager& fonts):
	nodeSprite(textures.getResourceReference(Textures_ID::Paper)),
	nodeName("Unnamed", fonts.getResourceReference(Fonts_ID::ArialNarrow), 18),
	objectId(0),
	size(nodeSprite.getTextureRect().width, nodeSprite.getTextureRect().height)
{
	nodeName.setOutlineColor(sf::Color::Black);
	nodeName.setOutlineThickness(1.f);
	centerOrigin(nodeSprite);
	centerOrigin(nodeName);
}

sf::FloatRect NodeEditorObject::getGlobalBounds() const
{
	const auto rectLeft = getPosition().x - nodeName.getOrigin().x * getScale().x;
	const auto rectTop = getPosition().y - nodeName.getOrigin().y * getScale().y;
	const auto rectWidth = nodeName.getLocalBounds().width * nodeName.getScale().x * getScale().x;
	const auto rectHeight = nodeName.getLocalBounds().height * nodeName.getScale().y * getScale().y;

	return sf::FloatRect(rectLeft, rectTop, rectWidth, rectHeight);
}

void NodeEditorObject::updateMouse(const sf::Vector2f& mousePosition)
{
	// After hovering with the mouse (executes once)
	if (!isSelected() && getGlobalBounds().contains(mousePosition))
		select();

	// After leaving with the mouse (executes once)
	if (isSelected() && !getGlobalBounds().contains(mousePosition))
		unselect();

	// Allows to move an object with the mouse when activated
	if(isActivated())
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			setPosition(mousePosition);
	}
}

void NodeEditorObject::update(sf::Time deltaTime)
{
	// Synchronises the position of the text on the object with the object
	nodeName.setPosition(getPosition());

	// Allows to move and edit the activated object
	if(isActivated())
	{
		// Faster moving
		auto additionalSpeed = 0.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
			additionalSpeed = 150.f;

		auto movingSpeed = this->movingSpeed + additionalSpeed;
		movingSpeed *= deltaTime.asSeconds();
		
		// Standard moving the object with keyboard
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			move(0, -movingSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			move(0, movingSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			move(-movingSpeed, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			move(movingSpeed, 0);

		auto rotationSpeed = this->rotationSpeed + additionalSpeed;
		rotationSpeed *= deltaTime.asSeconds();
		
		// Rotation of the object with keyboard
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
			setRotation(getRotation() + rotationSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
			setRotation(getRotation() - rotationSpeed);

		auto sizeChangeSpeed = this->sizeChangeSpeed + additionalSpeed;
		sizeChangeSpeed *= deltaTime.asSeconds();
		
		// Changing height of the object
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			setSize(getSize().x, getSize().y - sizeChangeSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			setSize(getSize().x, getSize().y + sizeChangeSpeed);

		// Changing width of the object
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			setSize(getSize().x + sizeChangeSpeed, getSize().y);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			setSize(getSize().x - sizeChangeSpeed, getSize().y);
	}
}

bool NodeEditorObject::isSelected() const noexcept
{
	return _isSelected;
}

void NodeEditorObject::select() noexcept
{
	_isSelected = true;
}

void NodeEditorObject::unselect() noexcept
{
	_isSelected = false;
}

bool NodeEditorObject::isActivated() const noexcept
{
	return _isActivated;
}

void NodeEditorObject::activate()
{
	_isActivated = true;

	nodeSprite.setColor(sf::Color::Red);
}

void NodeEditorObject::deactivate()
{
	_isActivated = false;

	nodeSprite.setColor(sf::Color::White);
}

void NodeEditorObject::setId(unsigned id) noexcept
{
	objectId = id;
}

unsigned NodeEditorObject::getId() const noexcept
{
	return objectId;
}

void NodeEditorObject::setName(const std::string& name)
{
	nodeName.setString(name);
	centerOrigin(nodeName);
}

void NodeEditorObject::setSize(float width, float height)
{
	size.x = width;
	size.y = height;
	nodeSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	centerOrigin(nodeSprite);
}

sf::Vector2f NodeEditorObject::getSize() const
{
	return size;
}

void NodeEditorObject::setDestroyed() noexcept
{
	_isDestroyed = true;
}

bool NodeEditorObject::isDestroyed() const noexcept
{
	return _isDestroyed;
}

void NodeEditorObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(nodeSprite, states);
	target.draw(nodeName);
}

void NodeEditorObject::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
	{
		if (isSelected())
		{
			if (!isActivated())
				activate();
		}
		else
		{
			if (isActivated())
				deactivate();
		}
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete)
	{
		if(_isActivated)
			setDestroyed();
	}
}
