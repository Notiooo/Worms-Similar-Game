#ifndef WEAPON_H
#define WEAPON_H

#include "SFML/Window/Event.hpp"

class Weapon
{
public:
	
	void shoot();


private:
	sf::Keyboard::Key activate_button = sf::Keyboard::Space;
};

#endif // !WEAPON_H
