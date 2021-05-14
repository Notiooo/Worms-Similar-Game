#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include "../Resources/Resources.h"

/**
 * \brief Allows to play music from assign audio files to identifiers.
 * Playback takes place by entering an identifier. Music is streamed on regular basis,
 * so it must be available all the time.
 */
class MusicPlayer : sf::NonCopyable
{
public:
	/**
	 * \brief Loads available songs
	 */
	MusicPlayer();

	/**
	 * \brief Plays the song associated with the specified ID.
	 * \param musicID Identifier of the music track.
	 */
	void play(Music_ID musicID);

	
	/**
	 * \brief Stops the currently playing song.
	 */
	void stop();
	
	/**
	 * \brief Pauses or unpauses the song.
	 * \param paused Pauses if true, unpauses otherwise
	 */
	void setPaused(bool paused);
	
	/**
	 * \brief Sets the volume from 0 (muted) to 100 (loudest).
	 * \param volume Music volume from 0 to 100
	 */
	void setVolume(float volume);


	/**
	 * \brief It returns currently playing (or paused) music
	 * \return Identifier of currently playing music
	 */
	Music_ID getCurrentMusic() const noexcept;

	
	/**
	 * \brief Plays the same song again from beginning
	 * \warning If no music was set, nothing happens
	 */
	void reset();

private:
	sf::Music music; //!< Streamed music from the audio file
	std::map<Music_ID, std::string> filePaths; //!< Assigns a music identifier to the corresponding file path
	float volume = 100.f; //!< Music volume from 0 to 100
	Music_ID currentMusic = Music_ID::None; //!< Identifier of currently playing music
};




#endif