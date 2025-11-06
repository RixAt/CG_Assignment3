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
// Description: Game controller/interface. Declares the Game class that
// owns simulation state (robots, bullets, effects), multi-viewport
// rendering, cameras, input handling (keyboard/mouse), scoring/timer,
// and simple audio hooks.
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

// Forward declarations
struct Vector3;
class Camera;
class Robot;
class Bullet;
struct ImpactFX;

// Enum Class: GameState, high-level game state
//   ShowIntro - Intro/menu screen
//   Playing - Active gameplay
//   RoundOver - Round has ended, show results
enum class GameState { ShowIntro, Playing, RoundOver };

// Enum Class: BulletSpeed, speed modes for bullets
//   Slow - Slow speed
//   Fast - Fast speed
//   VeryFast - Very fast speed
enum class BulletSpeed {Slow, Fast, VeryFast };
// Class: Game
// The main application object, owns world objects and the loop.
class Game {
public:
	// Struct: ViewportsLayout
	// Group of viewports for multi-viewport rendering
	struct ViewportsLayout {
		Viewport vpMain;
		Viewport vpInset;
		Viewport vpHUD;
	};
	// Struct: ArcballState
	// Arcball state for ESV camera control
	struct ArcballState {
		bool rotating = false;
		bool zooming = false;
		int lastX = 0, lastY = 0;
		float rotSens = 0.008f;
		float zoomSens = 0.1f;
		float minR = 20.0f, maxR = 500.0f;
	};
	// Struct: HudMetrics
	// Precomputed HUD layout metrics
	struct HudMetrics {
		int padX;        // outer horizontal margin
		int padY;        // outer vertical margin
		int lineStep;    // vertical spacing between lines
		int col1X;       // left column X
		int col2X;       // middle/right column X
		int col3X;       // far-right column X
	};
	// Construct with default toggles, heavy lifting done in init()
	Game();

	// Initialize game scene and audio; spawns robots, readies bullets, sets cameras
	void init();

	// Simulation update over time delta dt (in seconds)
	// Advances robots, bullets, effects, checks round end conditions
	// dt: Time delta in seconds
	void update(float dt);

	// Render the game scene to the window of size (winW x winH)
	// Computes viewports and draws each
	// winW: Window width in pixels
	// winH: Window height in pixels
	void draw(int winW, int winH) const;

	// Key handler for GLUT, handles toggles, fire, reset, etc.
	void handleKey(unsigned char key);

	// Special key handler for GLUT (F-keys and arrow keys)
	void handleSpecialKey(int key);
	// Mouse button handler for GLUT for arcball control of ESV camera in main viewport
	void handleMouseButton(int button, int state, int x, int y);
	// Mouse motion handler for GLUT for arcball control of ESV camera in main viewport(rotate/zoom)
	void handleMouseMotion(int x, int y);

	// Ends the current round with success/failure state
	// success: true if round was successful, false otherwise
	void endRound(bool success);

	// Resets the round state: robots, timer, score, bullets
	void resetRound();

	// Resumes the game from menu state
	void resumeFromMenu();

	// Returns true if ESV camera is the current main render camera
	bool isESVMainActive() const;
	
	// Toggle fullscreen/windowed mode and restore previous window size
	void toggleFullscreen();

	// Text label for current bullet speed mode (for HUD)
	const char* bulletSpeedLabel() const;

	// Fire a bullet from the specified camera's position and orientation
	void fireBulletFromCamera(const Camera& cam);

	// Event hooks (unused at this time)
	void onBulletFired();
	//void onRobotKilled();
	void onBulletMiss();

private:
	// Draws the world objects (robots, bullets, effects)
	void drawWorld() const;

	// Draws camera debug frustums and markers, excluding the specified camera
	void drawCameraDebugLines(const Camera* exclude, float aspect) const;

	// Computes the viewport layout for the given window size
	ViewportsLayout computeViewports(int winW, int winH) const;

	// Draws the specified viewport
	void drawMainViewport(const Viewport& vp) const;
	void drawInsetViewport(const Viewport& vp) const;
	void drawHUDViewport(const Viewport& vp) const;

	// Updates the ESV camera based on arcball parameters
	void updateArcballCamera();

	// All cameras used by the game
	CameraSystem cams;

	// Whether to draw camera frustums and markers
	bool showCameraFrustums = true;
	bool showCameraMarkers = true;

	// Arcball state for ESV camera control
	mutable ArcballState arcball;

	// Fullscreen state
	bool isFullscreen = false;
	// Last window size before going fullscreen
	int prevWinX = 100, prevWinY = 100;
	int prevWinW = 640, prevWinH = 480;

	// World
	// Enemy robots in the scene
	std::vector<Robot*> robots;

	// Bullets in the scene
	std::vector<Bullet> bullets;

	// Impact effect list for bullet hits
	std::vector<ImpactFX> impacts;

	// Maximum number of bullets in the pool
	static constexpr size_t maxBullets = 128;

	// Global render mode for geometry
	RenderMode g_renderMode;

	// Debug/visual toggles
	bool showAxes;
	bool showColliders;

	// Motion toggle
	bool motionEnabled;

	// Score and counters for HUD and victory conditions
	int score = 0;
	int robotsKilled = 0;
	int shotsFired = 0;
	int shotsHit = 0;

	// Round time remaining
	float timeRemaining = 30.0f; // seconds

	// Mission success flag
	bool missionSuccess = false;

	// Current game state
	GameState gameState{ GameState::ShowIntro };

	// Whether to show instructions on HUD
	bool showInstructions = true;

	// Current bullet speed mode
	BulletSpeed bulletSpeedMode = BulletSpeed::Slow;

	// Returns the numeric current bullet speed based on the selected mode
	inline float currentBulletSpeed() const {
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
	
	// Computes HUD layout metrics based on viewport size
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

	// Computes and returns the player's shooting accuracy percentage
	float accuracyPercentage() const {
		return shotsFired ? (100.0f * float(shotsHit) / float(shotsFired)) : 0.0f;
	}

	// Returns true if the round is over
	bool isRoundOver() const { return gameState == GameState::RoundOver; };
};


#endif // !GAME_HPP
