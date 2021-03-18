#ifndef WORMMOVEABLESTATE_H
#define WORMMOVEABLESTATE_H
#include "../../../../States/State.h"
#include "Worm.h"

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

/* This class is abstract, that it why
the constructor is protected */
class WormMoveableState : public State
{
protected:
	WormMoveableState(StateStack&, Worm&);
public:

	virtual void draw() const = 0;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

	virtual bool update(sf::Time) override = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

	// === Useful functions == //
	bool facingRight();


protected:
	void handleMovement(const sf::Event& event);
	void updateMovement(sf::Time deltatime);

	// 1 if looks right, -1 if looks left
	// Thanks to this I can easily set some vectors
	// so they're pointing the proper direction
	int direction;
	Worm& worm;

};
#endif