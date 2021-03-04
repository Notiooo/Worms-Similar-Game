#ifndef RESOURCES_H
#define RESOURCES_H

#include "SFML/Graphics/Texture.hpp"
#include "ResourceManager.h"

// ====== Textures ======= //

enum class Textures_ID
{
	AnExamplaryWorm,
};

using TextureManager = ResourceManager<sf::Texture, Textures_ID>;

#endif