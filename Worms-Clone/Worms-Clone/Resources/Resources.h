#ifndef RESOURCES_H
#define RESOURCES_H

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"

#include "ResourceManager.h"

// ====== Textures ======= //

/**
 * \brief Texture IDs under which individual textures can be saved and read.
 */
enum class Textures_ID
{
	// === Others === //
	WorldBackground,
	Water,
	Paper,
	AnExemplaryWorm,
	DeadWorm,
	HitWorm,
	Rope,

	// === TitleState === /
	TitleStateBackground,

	// === Weapons === //
	Bazooka,
	BazookaBullet,
	BazookaThumbnail,

	Cannon,
	CannonBullet,
	CannonThumbnail,

	Grenade,
	GrenadeBullet,
	GrenadeThumbnail,
	
	// === Inventory ===/
	Inventory,
};

/**
 * \brief Object storing textures of the game
 */
using TextureManager = ResourceManager<sf::Texture, Textures_ID>;

// ====== Fonts ======= //

/**
 * \brief Fonts IDs under which individual font can be saved and read.
 */
enum class Fonts_ID
{
	ArialNarrow,
};

/**
 * \brief Object storing fonts of the game
 */
using FontManager = ResourceManager<sf::Font, Fonts_ID>;

#endif