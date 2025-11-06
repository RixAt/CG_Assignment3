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
// File: sound.cpp
// Description: 
// ====================================================================

#include "sound.hpp"
#include <irrKlang\irrKlang.h>
#include <unordered_map>

namespace {
	irrklang::ISoundEngine* engine = nullptr;
	struct Track {
		irrklang::ISound* sound = nullptr;
		float volume = 1.0f;
	};
	std::unordered_map<std::string, Track> gMusic;
    inline float clamp01(float v) { return std::max(0.0f, std::min(1.0f, v)); }

}

namespace sound {
	void init() {
		if (!engine) {
			engine = irrklang::createIrrKlangDevice();
		}
	}

	void shutdown() {
		for (auto& pair : gMusic) {
			if (pair.second.sound) {
				pair.second.sound->stop();
				pair.second.sound->drop();
			}
		}
		gMusic.clear();
		if (engine) { engine->drop(); engine = nullptr; }

	}
    bool playMusicLoopOn(const std::string& channel,
        const std::string& path,
        float volume) {
        if (!engine) return false;

        // Stop/replace existing
        auto& tr = gMusic[channel];
        if (tr.sound) { tr.sound->stop(); tr.sound->drop(); tr.sound = nullptr; }

        tr.sound = engine->play2D(path.c_str(), /*loop*/true, /*startPaused*/false, /*track*/true);
        if (!tr.sound) return false;

        tr.volume = clamp01(volume);
        tr.sound->setVolume(tr.volume);
        return true;
    }

    void stopMusicOn(const std::string& channel) {
        auto it = gMusic.find(channel);
        if (it == gMusic.end()) return;
        if (it->second.sound) { it->second.sound->stop(); it->second.sound->drop(); }
        gMusic.erase(it);
    }

    void pauseMusicOn(const std::string& channel, bool pause) {
        auto it = gMusic.find(channel);
        if (it == gMusic.end() || !it->second.sound) return;
        it->second.sound->setIsPaused(pause);
    }

    void setMusicVolumeOn(const std::string& channel, float volume) {
        auto it = gMusic.find(channel);
        if (it == gMusic.end() || !it->second.sound) return;
        it->second.volume = clamp01(volume);
        it->second.sound->setVolume(it->second.volume);
    }

    bool isMusicPlayingOn(const std::string& channel) {
        auto it = gMusic.find(channel);
        return (it != gMusic.end() && it->second.sound && !it->second.sound->isFinished());
    }

    void setMasterVolume(float volume) {
        if (engine) engine->setSoundVolume(clamp01(volume));
    }

    void playSFX(const std::string& path, float volume) {
        if (!engine) return;
        if (auto* s = engine->play2D(path.c_str(), false, false, true)) {
            s->setVolume(clamp01(volume));
            s->drop();
        }
    }


} // namespace sound

