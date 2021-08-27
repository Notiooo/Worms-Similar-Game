#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H
#include <list>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "Resources/Resources.h"


/**
 * \brief A class that plays the given sounds in the game space.
 */
class SoundPlayer : sf::NonCopyable
{
public:
	SoundPlayer();

	/**
	 * \brief Plays sound at the point the player is looking at
	 * \param sound The identifier of the sound to be played
	 */
	void play(Sound_ID sound);

	
	/**
	 * \brief Plays sound at the point at the given position
	 * \param sound The identifier of the sound to be played
	 * \param position The position at which the sound should be played
	 */
	void play(Sound_ID sound, sf::Vector2f position);

	/**
	 * \brief Returns the length of sound time for a particular identifier
	 * \param sound The identifier of the sound whose length we are checking
	 * \return Audio length for the specified identifier
	 */
	sf::Time getSoundLength(Sound_ID sound) const;
	
	/**
	 * \brief Deletes sounds that have already ended
	 */
	void removeFinishedSounds();

	/**
	 * \brief Sets the position of the virtual ear that listens for sounds
	 * \param position New virtual ear position
	 */
	void setListenerPosition(sf::Vector2f position);
	
	/**
	 * \brief Returns the position of the virtual ear that listens for sounds
	 * \return The current position of the virtual ear (listener)
	 */
	sf::Vector2f getListenerPosition() const;

private:
	SoundManager soundPaths; //!< A manager that maps sound IDs to their file paths

	// List is used instead of vector, as vector may reallocate
	// and this way invalidate them when they're playing
	// And many more similar aspects
	std::list<sf::Sound> soundList; //!< List of currently playing sounds

	const float listenerZ = 300.f; //!< The distance of the listener (virtual ear) from the two-dimensional game plane.
	const float attenuation = 8.f; //!< The ratio of how fast a sound fades away as a function of distance
	const float minDistance = 300.f; //!< The minimum distance from the virtual ear at which sound begins to fade (to become more silent).
	
};





#endif