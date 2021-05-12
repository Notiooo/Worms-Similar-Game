#include "MusicPlayer.h"

MusicPlayer::MusicPlayer()
{
	// I assign identifiers to their audio file paths
	filePaths[Music_ID::MainMenu] = "Resources/Sounds/Music/MainMenu.ogg";
	filePaths[Music_ID::GameState] = "Resources/Sounds/Music/GameState.ogg";
}

void MusicPlayer::play(Music_ID musicID)
{
	const auto musicIter = filePaths.find(musicID);

	// Check if such an identifier exist
	assert(musicIter != filePaths.cend());
	
	const auto& musicPath = musicIter->second;

	if (!music.openFromFile(musicPath))
		throw std::runtime_error("No such music path as: " + musicPath);

	music.setVolume(volume);
	music.setLoop(true);
	music.play();
}

void MusicPlayer::stop()
{
	music.stop();
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		music.pause();
	else
		music.play();
}

void MusicPlayer::setVolume(float volume)
{
	music.setVolume(volume);
}
