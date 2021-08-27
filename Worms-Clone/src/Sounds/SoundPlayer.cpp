#include "pch.h"
#include "Sounds/SoundPlayer.h"

#include <cmath>
#include <SFML/Audio/Listener.hpp>

SoundPlayer::SoundPlayer()
{	
	soundPaths.storeResource(Sound_ID::Explosion, "resources/Sounds/explosion_sound.wav");
	soundPaths.storeResource(Sound_ID::FloorHit, "resources/Sounds/particle_sound.wav");
	soundPaths.storeResource(Sound_ID::DefaultWeaponShoot, "resources/Sounds/weapon_default_sound.wav");
	soundPaths.storeResource(Sound_ID::WormWalking, "resources/Sounds/moving_worm_sound.wav");
}

void SoundPlayer::play(Sound_ID sound)
{
	// Always play in front of listener
	play(sound, getListenerPosition());
}

void SoundPlayer::play(Sound_ID sound, sf::Vector2f position)
{
	soundList.emplace_back(soundPaths.getResourceReference(sound));
	auto& snd = soundList.back();
	snd.setPosition(position.x, -position.y, 0.f);
	snd.setAttenuation(attenuation);

	// Pythagorean theorem
	snd.setMinDistance(sqrt(minDistance * minDistance + listenerZ * listenerZ));

	snd.play();
}

sf::Time SoundPlayer::getSoundLength(Sound_ID sound) const
{
	return soundPaths.getResourceReference(sound).getDuration();
}

void SoundPlayer::removeFinishedSounds()
{
	soundList.remove_if([](const auto& sound)
		{
			return sound.getStatus() == sf::Sound::Stopped;
		});
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(sf::Vector3f(position.x, -position.y, listenerZ));
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	const auto& position = sf::Listener::getPosition();
	return { position.x, position.y };
}
