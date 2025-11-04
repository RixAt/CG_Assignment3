// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____   ___  _   _ _   _ ____  
// / ___| / _ \| | | | \ | |  _ \ 
// \___ \| | | | | | |  \| | | | |
//  ___) | |_| | |_| | |\  | |_| |
// |____/ \___/ \___/|_| \_|____/ 
// 
// ====================================================================
// File: sound.hpp
// Description: 
// ====================================================================

#ifndef SOUND_HPP
#define SOUND_HPP

#include <string>

namespace irrklang {
	class ISoundEngine;
	class ISound;
}

namespace sound {
	// Initialize the sound engine
	void init();
	// Shutdown the sound engine
	void shutdown();
	// Play music loop on specific channel
	bool playMusicLoopOn(const std::string& channel,
		const std::string& path,
		float volume = 0.6f);

	// Control music on specific channel
	void stopMusicOn(const std::string& channel);
	void pauseMusicOn(const std::string& channel, bool pause);
	void setMusicVolumeOn(const std::string& channel, float volume);
	bool isMusicPlayingOn(const std::string& channel);

	inline bool playAmbient(const std::string& path, float vol = 0.5f) {
		return playMusicLoopOn("ambient", path, vol);
	}

	inline bool playBackground(const std::string& path, float vol = 0.6f) {
		return playMusicLoopOn("background", path, vol);
	}

	// Play a sound effect
	void playSFX(const std::string& filepath, float volume = 0.5f);
	// Stop looped background music
	void stopMusic();
	// Set master volume
	void setMasterVolume(float volume); // volume: 0.0 to 1.0
	

}


#endif // !SOUND_HPP
