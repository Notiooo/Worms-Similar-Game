#include "MenuState.h"
#include "../../GUI/Button.h"
#include "../../Nodes/Physical/NodePhysicalBody.h"
#include "../../Nodes/Physical/NodePhysicalSprite.h"

void MenuState::createBackgroundWorld(sf::Vector2f pos)
{
	float width = 200.f;
	float height = 30.f;
	float padding_x = 200.f;
	float padding_y = 100.f;
	float rotation = 30.f;
	// Kinda hardcoded as it is just a menu
	for (int i = 0; i < 4; ++i)
	{
		std::unique_ptr<NodePhysicalSprite> staticPaperBlock = std::make_unique<NodePhysicalSprite>(
			World, NodePhysicalBody::Physical_Types::Static_Type,
			textures.getResourceReference(Textures_ID::Paper), pos,
			sf::Vector2f{ width, height });
		staticPaperBlock->setRotation(rotation);

		pos.y += std::sinf(NodePhysicalBase::angleToRadians(rotation)) * width + height + padding_y;
		rotation = 180 - rotation;
		pos.x += padding_x;
		padding_x = -padding_x;
		rootScene.pinNode(std::move(staticPaperBlock));
	}
}

MenuState::MenuState(StateStack& stack, const FontManager& fonts, sf::RenderWindow& window) :
	State(stack),
	World(b2Vec2(0, 9.8f))
{
	loadResources();
	backgroundTexture.setTexture(textures.getResourceReference(Textures_ID::WorldBackground));
	backgroundTexture.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));

	createBackgroundWorld(sf::Vector2f(window.getSize().x / 1.5f, 0.f));
	
	auto button = std::make_unique<GUI::Button>(textures, fonts, window);
	button->setText("PLAY!");
	button->matchSizeToText(20.f);
	button->setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
	button->setActiveFunction([this]()
		{
			requestPop();
			requestPush(State_ID::GameState);
		});
	buttons.store(std::move(button));
}

void MenuState::draw() const
{
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(backgroundTexture);
	rootScene.draw(target, states);

	buttons.draw(target, states);

}

bool MenuState::update(sf::Time deltaTime)
{
	buttons.update();

	rootScene.update(deltaTime);

	rootScene.removeDestroyed();

	// Update the physical world
	World.Step(1 / 60.f, 8, 3);

	return false;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	buttons.handleEvents(event);

	return false;
}

void MenuState::loadResources()
{
	textures.storeResource(Textures_ID::Paper, "Resources/Textures/World/paper_texture.png");
	textures.storeResource(Textures_ID::WorldBackground, "Resources/Textures/World/background_texture.png");
	textures.getResourceReference(Textures_ID::Paper).setRepeated(true);
	textures.getResourceReference(Textures_ID::WorldBackground).setRepeated(true);
}
