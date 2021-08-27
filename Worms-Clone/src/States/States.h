#ifndef STATES_H
#define STATES_H

/**
 * \brief Identifiers of individual states that can be used.
 */
enum class State_ID
{
	// === Application flow states === //
	TitleState, //!< Title page with pre-game information
	GameState, //!< A state that is a game in which the player can actively participate
	MenuState, //!< The main menu state in which the player can change the game settings and start the editor or the game, or close the game
	PauseState, //!< Pause state in which the player can pause the game for a while or return to the main menu, or even close the game
	EditorState, //!< The state in which the player can edit/create the current game world map


	// === Inside-game objects flow states === //
	
	// Worm
	WormHideState, //!< Player have ... seconds to hide
	WormPlayState, //!< Player can play his turn
	WormWaitState, //!< Player can't move with this worm
	WormHitState,  //!< State after getting hit by other player
	WormInventoryState, //!< State in which player can choose a weapon

};


#endif