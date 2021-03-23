#ifndef STATES_H
#define STATES_H

enum class State_ID
{
	// Application flow states
	TitleState,
	GameState,


	// == Inside-game objects flow states == //
	
	// Worm
	WormHideState, // Player have ... seconds to hide
	WormPlayState, // Player can play his turn
	WormWaitState, // Player can't move with this worm
	WormHitState,  // State after getting hit by other player
	WormInventoryState, // State in which player can choose a weapon

};


#endif