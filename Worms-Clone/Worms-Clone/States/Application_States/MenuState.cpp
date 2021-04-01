#include "MenuState.h"

#include <random>


#include "../../utils.h"
#include "../../GUI/Button.h"
#include "../../Nodes/Physical/NodePhysicalBody.h"
#include "../../Nodes/Physical/NodePhysicalSprite.h"
#include "../../Nodes/Physical/Specified/Worm/Worm.h"
#include "../../Nodes/Physical/Specified/Worm/Weapons/Delayed_Bullet.h"

MenuState::MenuState(StateStack& stack, const FontManager& fonts, sf::RenderWindow& window, int& _wormAmount, int& _numberOfTeams) :
	State(stack),
	window(window),
	fonts(fonts),
	World(b2Vec2(0, 9.8f)),
	gameName("Worms Clone", fonts.getResourceReference(Fonts_ID::ArialNarrow), 90),
	author("made by Dawid Grobert", fonts.getResourceReference(Fonts_ID::ArialNarrow), 30),
	amountText("How many worms per team?", fonts.getResourceReference(Fonts_ID::ArialNarrow), 21),
	amountTeams("How many teams?", fonts.getResourceReference(Fonts_ID::ArialNarrow), 21),
	wormsPerTeam(_wormAmount),
	numberOfTeams(_numberOfTeams)
{
	loadResources();
	backgroundTexture.setTexture(textures.getResourceReference(Textures_ID::WorldBackground));
	backgroundTexture.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));

	sf::Vector2f menuPosition(window.getSize().x / 4.f, window.getSize().y / 4.f);
	float menuOffset = 0.f;

	createBackgroundWorld(sf::Vector2f(window.getSize().x / 1.5f, 0.f));
	centerOrigin(gameName);
	centerOrigin(author);
	gameName.setPosition(menuPosition.x, menuPosition.y + menuOffset);
	menuOffset += gameName.getLocalBounds().height;
	author.setPosition(menuPosition.x, menuPosition.y + menuOffset);
	menuOffset += author.getLocalBounds().height;
	
	auto play_button = std::make_unique<GUI::Button>(textures, fonts, window);
	play_button->setText("Play the game!");
	play_button->matchSizeToText(20.f);
	play_button->setPosition(menuPosition.x, menuPosition.y + menuOffset + 30.f);
	play_button->setActiveFunction([this](GUI::Button& self)
		{
			requestPop();
			requestPush(State_ID::GameState);
		});

	amountText.setPosition(play_button->getPosition().x,
		play_button->getPosition().y + play_button->getLocalBounds().height + amountText.getLocalBounds().height / 2.f);
	centerOrigin(amountText);
	
	auto noWormsPerTeam = std::make_unique<GUI::Button>(textures, fonts, window);
	noWormsPerTeam->setText(std::to_string(wormsPerTeam) + " worms per team");
	noWormsPerTeam->matchSizeToText(20.f);
	noWormsPerTeam->setPosition(amountText.getPosition().x,
		amountText.getPosition().y + amountText.getLocalBounds().height + noWormsPerTeam->getLocalBounds().height / 2.f);
	centerOrigin(amountText);
	noWormsPerTeam->setActiveFunction([this](GUI::Button& self)
		{
			if (wormsPerTeam < maxWormAmount)
			{
				++wormsPerTeam;
				self.setText(std::to_string(wormsPerTeam) + " worms per team");
			}
		});
	noWormsPerTeam->setDeactiveFunction([this](GUI::Button& self)
		{
			if(wormsPerTeam > minWormAmount)
			{
				--wormsPerTeam;
				self.setText(std::to_string(wormsPerTeam) + " worms per team");
			}
		});

	amountTeams.setPosition(play_button->getPosition().x,
		noWormsPerTeam->getPosition().y + noWormsPerTeam->getLocalBounds().height + amountTeams.getLocalBounds().height / 2.f);
	centerOrigin(amountTeams);

	auto noOfTeams = std::make_unique<GUI::Button>(textures, fonts, window);
	noOfTeams->setText(std::to_string(numberOfTeams) + " teams");
	noOfTeams->matchSizeToText(20.f);
	noOfTeams->setPosition(amountTeams.getPosition().x,
		amountTeams.getPosition().y + amountTeams.getLocalBounds().height + noOfTeams->getLocalBounds().height / 2.f);
	centerOrigin(amountTeams);
	noOfTeams->setActiveFunction([this](GUI::Button& self)
		{
			if (numberOfTeams < maxTeamsAmount)
			{
				++numberOfTeams;
				self.setText(std::to_string(numberOfTeams) + " teams");
			}
		});
	noOfTeams->setDeactiveFunction([this](GUI::Button& self)
		{
			if (numberOfTeams > minTeamsAmount)
			{
				--numberOfTeams;
				self.setText(std::to_string(numberOfTeams) + " teams");
			}
		});
	
	buttons.store(std::move(play_button));
	buttons.store(std::move(noWormsPerTeam));
	buttons.store(std::move(noOfTeams));
	
}

void MenuState::createBackgroundWorld(sf::Vector2f pos)
{
	float width = 200.f;
	float height = 30.f;
	float padding_x = 200.f;
	float padding_y = 100.f;
	float rotation = 30.f;
	// Kinda hardcoded as it is just a menu
	for (int i = 0; i < 3; ++i)
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

	std::unique_ptr<NodePhysicalSprite> ground = std::make_unique<NodePhysicalSprite>(
		World, NodePhysicalBody::Physical_Types::Static_Type,
		textures.getResourceReference(Textures_ID::Paper), pos,
		sf::Vector2f{ 3000, 100 });
	rootScene.pinNode(std::move(ground));

	std::unique_ptr<NodePhysicalSprite> worm = std::make_unique<NodePhysicalSprite>(
		World, NodePhysicalBody::Physical_Types::Dynamic_Type,
		textures.getResourceReference(Textures_ID::AnExemplaryWorm), sf::Vector2f(pos.x, pos.y - 100));
	rootScene.pinNode(std::move(worm));
}


void MenuState::createGrenades(sf::Vector2f pos)
{
	static sf::Clock clock;
	static sf::Time timePerGrenade = sf::seconds(0);
	static std::random_device r;

	static std::default_random_engine e(r());
	static std::uniform_int_distribution<int> g(-50, 200); // random x position
	static std::uniform_int_distribution<int> t(3,6); // time per grenade spawn
	static std::uniform_int_distribution<int> te(4, 9); // time for explosion timer


	if(clock.getElapsedTime() > timePerGrenade)
	{
		clock.restart();

		std::unique_ptr<Delayed_Bullet> bullet = std::make_unique<Delayed_Bullet>(World, fonts, sf::Vector2f(pos.x + g(e), pos.y), textures.getResourceReference(Textures_ID::GrenadeBullet), 0, 0, sf::seconds(te(e)));
		bullet->setSparkColor(sf::Color::Cyan);
		rootScene.pinNode(std::move(bullet));
		timePerGrenade = sf::seconds(t(e));
	}
	
}

void MenuState::draw() const
{
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(backgroundTexture);
	rootScene.draw(target, states);
	target.draw(gameName, states);
	target.draw(author, states);
	target.draw(amountText);
	target.draw(amountTeams);

	
	buttons.draw(target, states);

}

bool MenuState::update(sf::Time deltaTime)
{
	buttons.update();

	rootScene.update(deltaTime);

	rootScene.removeDestroyed();

	createGrenades(sf::Vector2f(window.getSize().x / 1.5f, -100.f));

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
	textures.storeResource(Textures_ID::AnExemplaryWorm, "Resources/Textures/An_example_worm.png");
	textures.storeResource(Textures_ID::GrenadeBullet, "Resources/Textures/Weapons/grenade_bullet.png");
	textures.getResourceReference(Textures_ID::Paper).setRepeated(true);
	textures.getResourceReference(Textures_ID::WorldBackground).setRepeated(true);
}
