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
#include "effects.hpp"

class Game {
public:
	// World state
	std::vector<Robot*> robots;
	std::vector<ImpactFX> impacts;

	CameraSystem cams;

	// Rendering state
	RenderMode g_renderMode;
	bool showAxes;
	bool showColliders;
	bool motionEnabled;
	bool missionSuccess = false;
	bool showInstructions = true;

	Game();

	void init();
	void update(float dt);
	void draw(int winW, int winH) const;

	void drawWorld() const;

	// Keyboard input handling
	void handleKey(unsigned char key);
	void handleSpecialKey(int key);
	void handleMouseButton(int button, int state, int x, int y);
	void handleMouseMotion(int x, int y);

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

	void fireBulletFromCamera(const Camera& cam);
	void resumeFromMenu();

	struct ArcballState {
		bool rotating = false;
		bool zooming = false;
		int lastX = 0, lastY = 0;
		float rotSens = 0.008f;
		float zoomSens = 0.1f;
		float minR = 20.0f, maxR = 500.0f;
	} arcball;
	void updateArcballCamera();
	bool isESVMainActive() const;

	struct HudMetrics {
		int padX;        // outer horizontal margin
		int padY;        // outer vertical margin
		int lineStep;    // vertical spacing between lines
		int col1X;       // left column X
		int col2X;       // middle/right column X
		int col3X;       // far-right column X
	};

	static inline HudMetrics ComputeHudMetrics(const Viewport& vp) {
		HudMetrics m{};
		// margins as ~2–3% of viewport; clamp to sensible minimums
		m.padX = std::max(8, (int)std::round(vp.width * 0.02f));
		m.padY = std::max(10, (int)std::round(vp.height * 0.03f));

		// line height scales with viewport height; clamp for legibility w/ GLUT bitmap fonts
		m.lineStep = std::max(14, (int)std::round(vp.height * 0.032f));

		// columns as fractions of viewport width (tweak to taste)
		m.col1X = m.padX;                    // left
		m.col2X = (int)std::round(vp.width * 0.50f); // middle/right
		m.col3X = (int)std::round(vp.width * 0.72f); // far-right

		return m;
	}

private:
	enum class GameState {
		Playing,
		RoundOver,
		ShowIntro
	};
	GameState gameState = GameState::ShowIntro;

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
	BulletSpeed bulletSpeedMode = BulletSpeed::Slow;

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

	

	int score = 0;
	int robotsKilled = 0;
	float timeRemaining = 30.0f; // seconds
	int shotsFired = 0;
	int shotsHit = 0;


	float accuracyPercentage() const {
		return shotsFired ? (100.0f * float(shotsHit) / float(shotsFired)) : 0.0f;
	}

	bool isRoundOver() const { return gameState == GameState::RoundOver; };

	void onBulletFired();
	void onRobotKilled();
	void onBulletMiss();
	void endRound(bool success);
	void resetRound();



	const char* bulletSpeedLabel() const;
	
};


#endif // !GAME_HPP
