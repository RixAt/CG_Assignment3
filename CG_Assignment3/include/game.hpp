// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//   ____    _    __  __ _____ 
//  / ___|  / \  |  \/  | ____|
// | |  _  / _ \ | |\/| |  _|  
// | |_| |/ ___ \| |  | | |___ 
//  \____/_/   \_\_|  |_|_____|
// 
// ====================================================================
// File: game.hpp
// Description: 
// 
// ====================================================================

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include "utilities.hpp"
#include "render_util.hpp"
#include "robots.hpp"
#include "camera.hpp"
#include "bullet.hpp"

class Game {
public:
	// World state
	std::vector<Robot*> robots;

	CameraSystem cams;

	// Rendering state
	RenderMode g_renderMode;
	bool showAxes;
	bool showColliders;
	bool motionEnabled;

	Game();

	void init();
	void update(float dt);
	void draw(int winW, int winH) const;

	void drawWorld() const;

	// Keyboard input handling
	void handleKey(unsigned char key);
	void handleSpecialKey(int key);

	struct ViewportsLayout {
		Viewport vpMain;
		Viewport vpInset;
		Viewport vpHUD;
	};

	ViewportsLayout computeViewports(int winW, int winH) const;
	void drawMainViewport(const Viewport& vp) const;
	void drawInsetViewport(const Viewport& vp) const;
	void drawHUDViewport(const Viewport& vp) const;

	bool showCameraFrustums = true;
	bool showCameraMarkers = true;
	void drawCameraDebugLines(const Camera* exclude, float aspect) const;

	void toggleFullscreen();

private:
	enum class GameState {
		Playing,
		RoundOver
	};
	GameState gameState = GameState::Playing;

	bool isFullscreen = false;
	// Last window size before going fullscreen
	int prevWinX = 100, prevWinY = 100;
	int prevWinW = 640, prevWinH = 480;

	std::vector<Bullet> bullets;
	static constexpr size_t maxBullets = 128;

	enum class BulletSpeed {
		Slow,
		Fast,
		VeryFast
	};
	BulletSpeed bulletSpeedMode = BulletSpeed::Fast;

	float currentBulletSpeed() const {
		switch (bulletSpeedMode) {
			case BulletSpeed::Slow:
				return 30.0f;
			case BulletSpeed::Fast:
				return 60.0f;
			case BulletSpeed::VeryFast:
				return 120.0f;
		}
		return 30.0f; // Default
	}

	void fireBulletFromCamera(const Camera& cam);
	void updateBullets(float dt);
	void drawBullets(RenderMode mode) const;

	int score = 0;
	int robotsKilled = 0;
	float timeRemaining = 30.0f; // seconds
	int shotsFired = 0;
	int shotsHit = 0;

	int killStreak = 0;
	float streakWindow = 3.0f; // seconds
	float timeSinceLastKill = 999.9f;

	float accuracyPercentage() const {
		return shotsFired ? (100.0f * float(shotsHit) / float(shotsFired)) : 0.0f;
	}

	bool isRoundOver() const { return gameState == GameState::RoundOver; };

	void onBulletFired();
	void onRobotKilled();
	void onBulletMiss();
	void endRound();
	void resetRound();



	const char* bulletSpeedLabel() const;
	
};


#endif // !GAME_HPP
