#ifndef RESOURCES_H
#define RESOURCES_H

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"

#include "ResourceManager.h"

// ====== Textures ======= //

enum class Textures_ID
{
	World_Background,
	AnExamplaryWorm,
	Rope,

	// TitleState
	TitleState_Background,

	// Weapons
	Bazooka,
	Bazooka_Bullet,
};

using TextureManager = ResourceManager<sf::Texture, Textures_ID>;

// ====== Fonts ======= //

enum class Fonts_ID
{
	Arial_Narrow,
};

using FontManager = ResourceManager<sf::Font, Fonts_ID>;

#endif