#include "pch.h"
#include "States/Application_States/MenuState.h"

#include <cmath>
#include <random>

#include "GUI/Button.h"
#include "Nodes/Physical/NodePhysicalBody.h"
#include "Nodes/Physical/NodePhysicalSprite.h"
#include "Nodes/Physical/Specified/Worm/Worm.h"
#include "Nodes/Physical/Specified/Worm/Weapons/Delayed_Bullet.h"
#include "Utils/utils.h"

MenuState::MenuState(StateStack& stack, const FontManager& fonts, sf::RenderWindow& window, int& _wormAmount,
                     int& _numberOfTeams, MusicPlayer& music) :
	State(stack),
	fonts(fonts),
	window(window),
	availableResolutions{sf::Vector2u(1024, 768), {1280, 720}, {1366, 768}, {1680, 1050}, {1920, 1080}},
	buttons(window),
	gameName("Worms Clone", fonts.getResourceReference(Fonts_ID::ArialNarrow), 90),
	author("made by Dawid Grobert", fonts.getResourceReference(Fonts_ID::ArialNarrow), 30),
	amountText("How many worms per team?", fonts.getResourceReference(Fonts_ID::ArialNarrow), 21),
	amountTeams("How many teams?", fonts.getResourceReference(Fonts_ID::ArialNarrow), 21),
	wormsPerTeam(_wormAmount),
	numberOfTeams(_numberOfTeams),
	World(b2Vec2(0, 9.8f))
{
	// Plays the music until it is changed or stopped (actually it is changed only in gamestate)
	// The condition allows to not reset the music if it was playing already
	if(music.getCurrentMusic() != Music_ID::MainMenu)
		music.play(Music_ID::MainMenu);
	
	// It makes sure that the view is at proper place
	window.setView(window.getDefaultView());

	const auto currentResolution = window.getSize();
	selectedResolution = std::find(availableResolutions.cbegin(), availableResolutions.cend(), currentResolution);
	if (selectedResolution == availableResolutions.cend())
		selectedResolution = availableResolutions.cbegin();

	loadResources();
	backgroundTexture.setTexture(textures.getResourceReference(Textures_ID::WorldBackground));
	backgroundTexture.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));


	const sf::Vector2f titlePosition(window.getSize().x / 4.f, window.getSize().y / 6.f);

	createBackgroundWorld(sf::Vector2f(window.getSize().x / 1.5f, 0.f));
	centerOrigin(gameName);
	centerOrigin(author);
	gameName.setPosition(titlePosition.x, titlePosition.y);
	setPositionBelow(author, gameName);

	createButtons(window, getPositionBelow(author));
}

void MenuState::createBackgroundWorld(sf::Vector2f pos)
{
	// Properties of the ramps
	const auto width = 200.f;
	const auto height = 30.f;
	auto paddingX = 200.f;
	auto paddingY = 100.f;
	auto rotation = 30.f;

	// Kinda hardcoded as it is just a menu
	while (pos.y + sinf(NodePhysicalBase::angleToRadians(rotation)) * width + height + paddingY < window.getSize().
		y)
	{
		std::unique_ptr<NodePhysicalSprite> staticPaperBlock = std::make_unique<NodePhysicalSprite>(
			World, NodePhysicalBody::Physical_Types::Static_Type,
			textures.getResourceReference(Textures_ID::Paper), pos,
			sf::Vector2f{width, height});
		staticPaperBlock->setRotation(rotation);

		pos.y += sinf(NodePhysicalBase::angleToRadians(rotation)) * width + height + paddingY;
		rotation = 180 - rotation;
		pos.x += paddingX;
		paddingX = -paddingX;
		rootScene.pinNode(std::move(staticPaperBlock));
	}


	// Create the main ground of the Menu

	// NodePhysicalSprite's are centered origin, so I need to calculate their position properly

	// Properties of the ground
	const auto groundY = window.getSize().y - pos.y;

	auto ground = std::make_unique<NodePhysicalSprite>(
		World, NodePhysicalBody::Physical_Types::Static_Type,
		textures.getResourceReference(Textures_ID::Paper), sf::Vector2f(pos.x, pos.y + groundY / 2.f),
		sf::Vector2f{3000, groundY});
	rootScene.pinNode(std::move(ground));

	auto worm = std::make_unique<NodePhysicalSprite>(
		World, NodePhysicalBody::Physical_Types::Dynamic_Type,
		textures.getResourceReference(Textures_ID::AnExemplaryWorm), sf::Vector2f(pos.x, pos.y - 100));
	rootScene.pinNode(std::move(worm));
}


void MenuState::createGrenades(sf::Vector2f pos)
{
	static sf::Clock clock;
	static auto timePerGrenade = sf::seconds(0);
	static std::random_device r;

	static std::default_random_engine e(r());
	static std::uniform_int_distribution<int> offsetX(-50, 200); // random x position
	static std::uniform_int_distribution<int> timePerSpawn(3, 6); // time per grenade spawn
	static std::uniform_int_distribution<int> timeExplosion(4, 9); // time for explosion timer


	if (clock.getElapsedTime() > timePerGrenade)
	{
		clock.restart();

		auto bullet = std::make_unique<Delayed_Bullet>(World, soundPlayer, fonts, sf::Vector2f(pos.x + offsetX(e), pos.y),
		                                               textures.getResourceReference(Textures_ID::GrenadeBullet),
		                                               textures, 0, 0, sf::seconds(timeExplosion(e)));
		bullet->setSparkColor(sf::Color::Cyan);
		rootScene.pinNode(std::move(bullet));
		timePerGrenade = sf::seconds(timePerSpawn(e));
	}
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// First it draws background
	target.draw(backgroundTexture);

	// Then the live background
	rootScene.draw(target, states);

	// At the end the whole GUI
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

	// Removes nodes to be removed (mainly grenades).
	rootScene.removeDestroyed();

	soundPlayer.removeFinishedSounds();

	// Creates grenades in the background
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

void MenuState::createButtons(sf::RenderWindow& window, sf::Vector2f position)
{
	// Play Button
	auto play_button = std::make_unique<GUI::Button>(textures, fonts);
	play_button->setText("Play the game!");
	play_button->matchSizeToText(20.f);
	play_button->setPosition(position.x, position.y + play_button->getLocalBounds().height);
	play_button->setActiveFunction([this](GUI::Button& self)
	{
		requestPop();
		requestPush(State_ID::GameState);
	});

	// Level editor
	auto levelEditor = std::make_unique<GUI::Button>(textures, fonts);
	levelEditor->setText("Level Editor");
	levelEditor->setSize(play_button->getGlobalBounds().width, play_button->getGlobalBounds().height);
	levelEditor->setPositionBelow(*play_button, 10.f);
	levelEditor->setActiveFunction([this](GUI::Button& self)
	{
		requestPop();
		requestPush(State_ID::EditorState);
	});

	// Exit button
	auto exitButton = std::make_unique<GUI::Button>(textures, fonts);
	exitButton->setText("Exit the game");
	exitButton->setSize(play_button->getGlobalBounds().width, play_button->getGlobalBounds().height);
	exitButton->setPositionBelow(*levelEditor, 10.f);
	exitButton->setActiveFunction([&window](GUI::Button& self)
	{
		window.close();
	});

	// Text with information that player can change her amount of worms per team
	centerOrigin(amountText);
	setPositionBelow(amountText, *exitButton);

	// Button where player can change amount of worm per team
	auto noWormsPerTeam = std::make_unique<GUI::Button>(textures, fonts);
	noWormsPerTeam->setText(std::to_string(wormsPerTeam) + " worms per team");
	noWormsPerTeam->matchSizeToText(20.f);
	noWormsPerTeam->setPositionBelow(amountText, 10.f);
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
		if (wormsPerTeam > minWormAmount)
		{
			--wormsPerTeam;
			self.setText(std::to_string(wormsPerTeam) + " worms per team");
		}
	});

	// Text with information that player can here change number of teams
	centerOrigin(amountTeams);
	setPositionBelow(amountTeams, *noWormsPerTeam);

	// Button that allow player to change number of teams
	auto noOfTeams = std::make_unique<GUI::Button>(textures, fonts);
	noOfTeams->setText(std::to_string(numberOfTeams) + " teams");
	noOfTeams->matchSizeToText(20.f);
	noOfTeams->setPositionBelow(amountTeams, 10.f);
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

	// Changing Resolution
	auto resolution = std::make_unique<GUI::Button>(textures, fonts);
	resolution->setText(
		"Resolution: " + std::to_string(selectedResolution->x) + "x" + std::to_string(selectedResolution->y));
	resolution->matchSizeToText(20.f);
	resolution->setPositionBelow(*noOfTeams, 30.f);
	resolution->setActiveFunction([this, &window](GUI::Button& self)
	{
		if (selectedResolution == availableResolutions.cend() - 1)
			selectedResolution = availableResolutions.cbegin();
		else
			++selectedResolution;

		window.create(sf::VideoMode(selectedResolution->x, selectedResolution->y), "Worms Clone",
		              sf::Style::Titlebar | sf::Style::Close | fullScreen);
		requestPop();
		requestPush(State_ID::MenuState);
	});
	resolution->setDeactiveFunction([this, &window](GUI::Button& self)
	{
		if (selectedResolution == availableResolutions.cbegin())
			selectedResolution = availableResolutions.cend() - 1;
		else
			--selectedResolution;

		window.create(sf::VideoMode(selectedResolution->x, selectedResolution->y), "Worms Clone",
		              sf::Style::Titlebar | sf::Style::Close | fullScreen);
		requestPop();
		requestPush(State_ID::MenuState);
	});

	// Set Fullscreen
	auto fullScreenButton = std::make_unique<GUI::Button>(textures, fonts);
	fullScreenButton->setText("Fullscreen: " + (fullScreen ? std::string("yes") : std::string("no")));
	fullScreenButton->matchSizeToText(20.f);
	fullScreenButton->setPositionBelow(*resolution, 10.f);
	fullScreenButton->setActiveFunction([this, &window](GUI::Button& self)
	{
		fullScreen = (fullScreen) ? 0 : sf::Style::Fullscreen;
		self.setText("Fullscreen: " + (fullScreen ? std::string("yes") : std::string("no")));
		window.create(sf::VideoMode(selectedResolution->x, selectedResolution->y), "Worms Clone",
		              sf::Style::Titlebar | sf::Style::Close | fullScreen);
	});

	buttons.store(std::move(fullScreenButton));
	buttons.store(std::move(resolution));
	buttons.store(std::move(play_button));
	buttons.store(std::move(noWormsPerTeam));
	buttons.store(std::move(noOfTeams));
	buttons.store(std::move(levelEditor));
	buttons.store(std::move(exitButton));
}

void MenuState::loadResources()
{
	textures.storeResource(Textures_ID::Paper, "resources/Textures/World/paper_texture.png");
	textures.storeResource(Textures_ID::WorldBackground, "resources/Textures/World/background_texture.png");
	textures.storeResource(Textures_ID::AnExemplaryWorm, "resources/Textures/An_example_worm.png");
	textures.storeResource(Textures_ID::GrenadeBullet, "resources/Textures/Weapons/grenade_bullet.png");
	textures.storeResource(Textures_ID::SmokeParticle, "resources/Textures/smokeParticle.png");
	textures.getResourceReference(Textures_ID::Paper).setRepeated(true);
	textures.getResourceReference(Textures_ID::WorldBackground).setRepeated(true);
}
