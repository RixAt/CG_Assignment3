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
// File: game.cpp
// Description: Core game loop/game controller for Robot Hunter.
// Owns world objects, handles updates and rendering, input handling,
// HUD, and game state.
// ====================================================================

#include "game.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "sound.hpp"

using namespace std;

// Ctor: Initialize simple game state, toggles
Game::Game()
	: g_renderMode(RenderMode::Solid),
	showAxes(true),
	showColliders(false),
	motionEnabled(true)
{
	// See init() for scene setup
}

// init(): Initialize game state
// Seeds random number generator and spawns robots
// Preps bullets pool, selects FPV camera for rendering and control
// Initializes sound system and starts ambient/background music
void Game::init() {
	srand(time(nullptr));
	robots.clear();

	// Spawns ten robots at random XY positions on ground plane
	for (int i = 0; i < 10; ++i) {
		float x = (std::rand() % 200 - 100);
		float z = (std::rand() % 200 - 100);
		Vector3 spawnPos(x, 0.0f, z);

		robots.push_back(new Robot(spawnPos,11.0f));
	}
	// Prepare bullets pool to reduce vector growth
	bullets.reserve(maxBullets);

	// Default to FPV camera for rendering and control
	cams.setRenderFPV();
	cams.controlCam = &cams.cameraFPV;

	// BONUS: Initialize sound system and start ambient/background music
	sound::init();
	sound::playAmbient("assets/ambience.ogg", 0.50f);
	sound::playBackground("assets/loop.ogg",0.15f);
}

// update(dt): Update game world state over time delta dt (in seconds)
// Updates robots, bullets, effects, checks round end conditions
void Game::update(float dt) {
	// Early out if showing intro or round over
	if (gameState == GameState::ShowIntro) return;
	if (gameState == GameState::RoundOver) return;
	// Update robots if motion enabled
	if (motionEnabled) {
		for (auto& r : robots) {
			r->update(dt);
		}
	}
	// Update effects
	effectsUpdate(impacts, dt);
	
	// Advance bullets and check for hits
	bulletsUpdate(bullets, dt, robots, robotsKilled, shotsHit, score, 3.0f, 100, &impacts);

	// Check if any robots are still alive
	bool anyAlive = false;
	for (const auto& r : robots) {
		if (r && r->isAlive()) {
			anyAlive = true;
			break;
		}
	}

	// Early mission completion check: all robots dead and at least 50% accuracy
	if (!anyAlive && timeRemaining > 0.0f && gameState == GameState::Playing) {
		if (accuracyPercentage() >= 50.0f) {
			endRound(true);
		}
		else {
			endRound(false);
		}
		return;
	}

	// Countdown time remaining and auto complete round if time expires based on conditions
	timeRemaining = std::max(0.0f, timeRemaining - dt);
	if (timeRemaining <= 0.0f && gameState == GameState::Playing) {
		bool allDead = !anyAlive;
		bool goodAccuracy = accuracyPercentage() >= 50.0f;

		endRound(allDead && goodAccuracy);
		return;
	}
}

// draw(): Render the game scene to the window of size (winW x winH)
// Sets up viewports and calls sub-draw methods
// Note: Depth test is managed per-viewport
void Game::draw(int winW, int winH) const {
	glDisable(GL_SCISSOR_TEST);
	glViewport(0, 0, winW, winH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);

	const ViewportsLayout viewports = computeViewports(winW, winH);

	drawMainViewport(viewports.vpMain);
	drawInsetViewport(viewports.vpInset);
	drawHUDViewport(viewports.vpHUD);

	const float aspect = (winH > 0) ? (float)winW / (float)winH : 1.0f;

	glutSwapBuffers();
}

// drawWorld(): Render the 3D world: ground plane, axes, robots
void Game::drawWorld() const {
	DrawGround();
	if (showAxes) {
		DrawAxes(100.0f);
	}

	for (const auto& r : robots) {
		r->draw(g_renderMode);

		if (showColliders) {
			r->drawColliderDebug();
		}
	}

}

// drawCameraDebugLines(): Draw debug lines for camera frustums and markers
// Excludes the specified camera from being drawn
void Game::drawCameraDebugLines(const Camera* exclude, float aspect) const {
	if (showCameraFrustums) {
		if (exclude != &cams.cameraFPV) {
			DrawCameraFrustum(cams.cameraFPV, aspect, 0.5f, Vector3(1.0f, 0.5f, 0.0f));
		}
		if (exclude != &cams.cameraESV) {
			DrawCameraFrustum(cams.cameraESV, aspect, 1.0f, Vector3(0.2f, 0.7f, 1.0f));
		}
		if (exclude != &cams.cameraFree) {
			//DrawCameraFrustum(cams.cameraFree, aspect, 0.5f, Vector3(0.5f, 1.0f, 0.5f));
		}
	}
	if (showCameraMarkers) {
		if (exclude != &cams.cameraFPV) {
			DrawCameraMarker(cams.cameraFPV, 2.0f, Vector3(1.0f, 0.5f, 0.0f));
		}
		if (exclude != &cams.cameraESV) {
			DrawCameraMarker(cams.cameraESV, 2.0f, Vector3(0.2f, 0.7f, 1.0f));
		}
		if (exclude != &cams.cameraFree) {
			//DrawCameraMarker(cams.cameraFree, 2.0f, Vector3(0.5f, 1.0f, 0.5f));
		}
	}
}

// computeViewports(): Compute viewport layouts for main, inset, and HUD viewports
// Viewport 1: (HUD) top band, height = 1/8 window height
// Viewport 2: (Main) remaining 7/8
// Viewport 3: (Inset) top-right corner of main viewport
Game::ViewportsLayout Game::computeViewports(int winW, int winH) const {
	ViewportsLayout allViewports{};

	const int hudH = std::max(1, winH / 8);
	const int mainH = std::max(1, winH - hudH);
	const int mainW = winW;

	// HUD viewport (full width, top band)
	allViewports.vpHUD.x = 0; allViewports.vpHUD.width = winW;
	allViewports.vpHUD.height = hudH;
	allViewports.vpHUD.y = winH - hudH;

	// Main viewport (remaining bottom area)
	allViewports.vpMain.x = 0;
	allViewports.vpMain.y = 0;
	allViewports.vpMain.width = mainW;
	allViewports.vpMain.height = mainH;

	// Inset viewport (top-right corner of main viewport)
	const int insetW = std::max(120, mainW / 4);
	const int insetH = std::max(90, mainH / 4);
	allViewports.vpInset.width = insetW;
	allViewports.vpInset.height = insetH;
	allViewports.vpInset.x = allViewports.vpMain.x + allViewports.vpMain.width - insetW - 10;
	allViewports.vpInset.y = allViewports.vpMain.y + allViewports.vpMain.height - insetH - 10;

	return allViewports;
}

// drawMainViewport(): Draw main 3D viewport
// Clears only this viewport region
// Applies current render camera (FPV or ESV/free depending on state)
// Renders world, bullets, and impact FX
// Optional camera debug overlays (except current)
// Draws crosshair/gun only in FPV
void Game::drawMainViewport(const Viewport& vp) const {
	SetViewport(vp);
	glClearColor(0.53f, 0.81f, 0.92f, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	cams.renderCam->applyView(vp.width, vp.height);
	drawWorld();
	bulletsDraw(bullets, g_renderMode);
	effectsDrawImpacts(impacts);

	const float aspect = (vp.height > 0) ? (float)vp.width / (float)vp.height : 1.0f;
	drawCameraDebugLines(cams.renderCam, aspect);

	// Draw crosshair at center of screen
	if (cams.renderCam == &cams.cameraFPV) {
		DrawCameraGun(*cams.renderCam);
		DrawCrosshair(vp.width, vp.height);
	}

}

// drawInsetViewport(): Draw inset 3D viewport
// Renders the alternate camera view (if main is FPV, inset is ESV, and vice versa)
void Game::drawInsetViewport(const Viewport& vp) const {
	SetViewport(vp);
	glClearColor(0.53f, 0.81f, 0.92f, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	const Camera* insetCam = (cams.renderCam == &cams.cameraFPV) ? &cams.cameraESV
							: (cams.renderCam == &cams.cameraESV) ? &cams.cameraFPV
							: &cams.cameraESV;
	
	insetCam->applyView(vp.width, vp.height);
	drawWorld();
	bulletsDraw(bullets, g_renderMode);
	effectsDrawImpacts(impacts);

	// Draw white border around inset viewport
	DrawViewportBorder(vp, Vector3(1.0f, 1.0f, 1.0f), 2.0f);

	// Show camera gun in inset if it's the FPV camera
	if (insetCam == &cams.cameraFPV) {
		DrawCameraGun(*insetCam);
	}

	const float aspect = (vp.height > 0) ? (float)vp.width / (float)vp.height : 1.0f;
	drawCameraDebugLines(insetCam
		, aspect);
}

// Draw Heads-Up Display (HUD) viewport
// Displays game information
void Game::drawHUDViewport(const Viewport& vp) const {
	SetViewport(vp);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, vp.width, 0, vp.height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);

	// Responsive coluimns based on viewport size
	const HudMetrics hudM = ComputeHudMetrics(vp);
	int yL = vp.height - hudM.padY;
	int yM = vp.height - hudM.padY;
	int yR = vp.height - hudM.padY;

	// Left column: title and mission info
	DrawText2D(hudM.col1X, yL, "Robot Hunter");
	yL -= (int)std::round(hudM.lineStep * 1.2f); // extra spacing
	DrawText2D(hudM.col1X, yL, "Obj: Eliminate all robots with >= 50% accuracy");
	yL -= hudM.lineStep;
	DrawText2D(hudM.col1X, yL, "Time Limit: 30s");
	yL -= hudM.lineStep;
	DrawText2D(hudM.col1X, yL, "Scoring: +100 hit, -150 miss");
	yL -= (int)std::round(hudM.lineStep * 1.2f); // extra spacing

	// Middle column: dynamic game stats
	DrawText2D(hudM.col2X, yM, ("Score: " + std::to_string(score)).c_str());
	yM -= hudM.lineStep;
	DrawText2D(hudM.col2X, yM, ("Robots Killed: " + std::to_string(robotsKilled)).c_str());
	yM -= hudM.lineStep;
	DrawText2D(hudM.col2X, yM, ("Remaining Time: " + std::to_string((int)std::ceilf(timeRemaining)) + " / 30").c_str());

	// Right column: bullet stats
	DrawText2D(hudM.col3X, yR, ("Bullet Speed: " + std::string(bulletSpeedLabel())).c_str());
	yR -= hudM.lineStep;
	DrawText2D(hudM.col3X, yR, ("Shots: " + std::to_string(shotsHit) + "/" + std::to_string(shotsFired)).c_str());
	yR -= hudM.lineStep;
	DrawText2D(hudM.col3X, yR, ("Accuracy: " + std::to_string((int)std::round(accuracyPercentage())) + "%").c_str());

	// Round result message
	if (isRoundOver()) {
		const char* resultText = (missionSuccess ? "Mission Successful! Press [R] to reset" : "Mission Failed! Press [R] to reset");
		yM -= hudM.lineStep;
		DrawText2D(hudM.col2X, yM, resultText, GLUT_BITMAP_HELVETICA_12);
	}

	if (!showInstructions) {
		// 
	}
	else {
		// Full overlay *within* Viewport 1
		// Dim background for readability
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.f, 0.f, 0.f, 0.85f);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);                glVertex2f(vp.width, 0);
		glVertex2f(vp.width, vp.height); glVertex2f(0, vp.height);
		glEnd();
		glDisable(GL_BLEND);
		glColor3f(1.0f, 1.0f, 1.0f);

		// Title
		DrawText2D(hudM.col1X, vp.height - hudM.padY, "Keyboard Controls (Press I to close) [PAUSED]");

		// Lines (fit comfortably in 1/8 height by using tight spacing)
		const char* lines[] = {
			"Keys: W wireframe | S solid | V vertices | C colliders",
			"B bullet speed | M motion | F1 fullscreen | F2 FPV/ESV",
			"A axes | Arrows move/turn | Space fire | ESC exit",
			"I toggle instructions/pause game"
		};

		const int N = (int)(sizeof(lines) / sizeof(lines[0]));

		// Layout metrics for overlay
		const int yTop = vp.height - hudM.padY - (int)std::round(hudM.lineStep * 1.6f);
		// Slightly tighter spacing to fit more lines in 1/8 height
		const int step = std::max(12, (int)std::round(hudM.lineStep * 0.9f));

		// Two columns: split list roughly in half
		const int perCol = (N + 1) / 2;

		// Column X positions: left margin, and a mid point within the HUD band
		const int colLeftX = hudM.col1X;
		const int colRightX = std::max(hudM.col1X + 220, (int)std::round(vp.width * 0.52f));

		// Draw left column
		int y = yTop;
		for (int i = 0; i < perCol; ++i) {
			DrawText2D(colLeftX, y, lines[i]);
			y -= step;
			if (y < hudM.padY) break; // safety for tiny windows
		}

		// Draw right column
		y = yTop;
		for (int i = perCol; i < N; ++i) {
			DrawText2D(colRightX, y, lines[i]);
			y -= step;
			if (y < hudM.padY) break;
		}
		//int y = vp.height - hudM.padY - (int)std::round(hudM.lineStep * 1.6f);
		//for (const char* s : lines) {
		//	DrawText2D(hudM.col1X, y, s);
		//	y -= hudM.lineStep;
		//	if (y < hudM.padY) break; // safety if the window is very shortdraw
		//}
	}

	// Draw white border around HUD viewport
	DrawViewportBorder(vp, Vector3(1.0f, 1.0f, 1.0f), 2.0f);

	// Restore matrices
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

// handleKey(): Handle standard key input
// Handles game state keys (reset, instructions) and in-game controls
void Game::handleKey(unsigned char key) {
	if (gameState == GameState::RoundOver || gameState == GameState::ShowIntro) {
		// While paused or round over, only allow reset, instructions toggle, and quit
		if (key == 'r' || key == 'R') {
			sound::playSFX("assets/click.ogg", 0.02f);
			resetRound();
			glutPostRedisplay();
			return;
		} 
		if (key == 'i' || key == 'I') {
			sound::playSFX("assets/click.ogg", 0.02f);
			showInstructions = !showInstructions;
			if (!showInstructions && gameState == GameState::ShowIntro) {
				gameState = GameState::Playing;
			}
			glutPostRedisplay();
			return;
		}
		if (key == 27) exit(0);
		return;
	}
	
	
	switch (key) {
	case 27: // ESC key
		sound::playSFX("assets/click.ogg", 0.05f);
		exit(0);
		break;
	case 'w':
	case 'W':
		sound::playSFX("assets/click.ogg", 0.05f);
		g_renderMode = RenderMode::Wireframe;
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		sound::playSFX("assets/click.ogg", 0.05f);
		g_renderMode = RenderMode::Solid;
		glutPostRedisplay();
		break;
	case 'v':
	case 'V':
		sound::playSFX("assets/click.ogg", 0.05f);
		g_renderMode = RenderMode::Vertices;
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		sound::playSFX("assets/click.ogg", 0.05f);
		showAxes = !showAxes;
		glutPostRedisplay();
		break;
	case 'c':
	case 'C':
		sound::playSFX("assets/click.ogg", 0.05f);
		showColliders = !showColliders;
		glutPostRedisplay();
		break;
	case 'i':
	case 'I':
		// Toggle instructions overlay and pause/resume game
		sound::playSFX("assets/click.ogg", 0.02f);
		showInstructions = !showInstructions;
		if (gameState == GameState::ShowIntro) {
			gameState = GameState::Playing;
		}
		else {
			gameState = GameState::ShowIntro;
		}
		glutPostRedisplay();
		break;
	case '3':
		// Switch to free camera
		sound::playSFX("assets/click.ogg", 0.05f);
		cams.activateFreeCam();
		break;
	case 'm':
	case 'M':
		// Toggle robot motion
		sound::playSFX("assets/click.ogg", 0.05f);
		motionEnabled = !motionEnabled;
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
		// Toggle camera frustums display
		sound::playSFX("assets/click.ogg", 0.05f);
		showCameraFrustums = !showCameraFrustums;
		glutPostRedisplay();
		break;
	case 'z':
	case 'Z':
		cams.controlCam->moveLeft(cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case 'x':
	case 'X':
		cams.controlCam->moveRight(cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case 'q':
	case 'Q':
		// Elevate camera up only if free camera is active
		cams.controlCam->elevate(cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		// Elevate camera down only if free camera is active
		cams.controlCam->elevate(-cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case 'r':
	case 'R':
		// Reset round
		sound::playSFX("assets/click.ogg", 0.05f);
		resetRound();
		glutPostRedisplay();
		break;
	case ' ':
		// Fire bullet from current camera (locked to FPV)
		if(gameState == GameState::Playing)
			fireBulletFromCamera(*cams.controlCam);
		glutPostRedisplay();
		break;
	case 'b':
	case 'B':
		// Cycle bullet speed
		switch (bulletSpeedMode) {
		case BulletSpeed::Slow:
			bulletSpeedMode = BulletSpeed::Fast;
			break;
		case BulletSpeed::Fast:
			bulletSpeedMode = BulletSpeed::VeryFast;
			break;
		case BulletSpeed::VeryFast:
			bulletSpeedMode = BulletSpeed::Slow;
			break;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
};

//handleSpecialKey(): Handle special key input (arrow keys, F-keys)
// F1: toggle fullscreen
// F2: toggle FPV/ESV view
// Arrow keys: move/turn camera
void Game::handleSpecialKey(int key) {
	// Early out if round over or showing intro
	if (gameState == GameState::RoundOver || gameState == GameState::ShowIntro) {
		return;
	}

	switch (key) {
	case GLUT_KEY_F1:
		// Toggle fullscreen
		sound::playSFX("assets/click.ogg", 0.05f);
		toggleFullscreen();
		break;
	case GLUT_KEY_F2:
		// Toggle view mode
		sound::playSFX("assets/click.ogg", 0.05f);
		cams.toggleFPV_ESV();
		if (cams.controlCam == &cams.cameraFree) {
			cams.controlCam = cams.renderCam;
		}
		
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		// Move camera forward
		cams.controlCam->moveForward(cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		// Move camera backward
		cams.controlCam->moveBackward(cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		// Rotate camera left
		cams.controlCam->turnLeft(cams.controlCam->turnSpeed);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		// Rotate camera right
		cams.controlCam->turnRight(cams.controlCam->turnSpeed);
		glutPostRedisplay();
		break;
	default:
		break;
	}
};	

// toggleFullscreen(): Toggle between fullscreen and windowed mode
void Game::toggleFullscreen() {
	if (!isFullscreen) {
		prevWinX = glutGet(GLUT_WINDOW_X);
		prevWinY = glutGet(GLUT_WINDOW_Y);
		prevWinW = glutGet(GLUT_WINDOW_WIDTH);
		prevWinH = glutGet(GLUT_WINDOW_HEIGHT);
		glutFullScreen();
		isFullscreen = true;
	}
	else {
		glutFullScreen();
		glutPositionWindow(prevWinX, prevWinY);
		glutReshapeWindow(prevWinW, prevWinH);
		isFullscreen = false;
	}
	glutPostRedisplay();
}

// bulletSpeedLabel(): Get string label for current bullet speed mode
const char* Game::bulletSpeedLabel() const {
	switch (bulletSpeedMode) {
	case BulletSpeed::Slow:
		return "Slow";
	case BulletSpeed::Fast:
		return "Fast";
	case BulletSpeed::VeryFast:
		return "Very Fast";
	default:
		return "Unknown";
	}
}

// endRound(): End the current round with success/failure
void Game::endRound(bool success) {
	
	// BONUS: Play different sound effects for win/lose
	if (success) {
		sound::playSFX("assets/win.ogg",1.0f);
	}
	else {
		sound::playSFX("assets/lose.ogg",0.90f);
	}
	gameState = GameState::RoundOver;
	missionSuccess = success;
}

// resetRound(): Reset game state for a new round
void Game::resetRound() {
	score = 0;
	robotsKilled = 0;
	timeRemaining = 30.0f;
	shotsFired = 0;
	shotsHit = 0;
	gameState = GameState::Playing;

	for (auto* r : robots) delete r;
	robots.clear();
	for (int i = 0; i < 10; ++i) {
		float x = (std::rand() % 200 - 100);
		float z = (std::rand() % 200 - 100);
		Vector3 spawnPos(x, 0.0f, z);
		robots.push_back(new Robot(spawnPos, 12.0f));
	}

	bullets.clear();

}

// onBulletFired(): Event handler for when a bullet is fired
// Not used currently
void Game::onBulletFired() {
	if (gameState == GameState::RoundOver) return;
	//shotsFired++;
}

// onBulletMiss(): Event handler for when a bullet miss
// Not used currently
void Game::onBulletMiss() {
	if (gameState == GameState::RoundOver) return;
}

// fireBulletFromCamera(): Fire a bullet from the specified camera's position and orientation
// Only works if game is active and camera is FPV
void Game::fireBulletFromCamera(const Camera& cam) {
	if (gameState == GameState::RoundOver) return;
	if (!cams.controlCam) return;

	Vector3 eye, right, up, forward;
	cam.getEyeBasis(eye, right, up, forward);

	// Offset start position slightly forward from camera eye
	const Vector3 start = eye + forward * 1.5f;
	const float speed = currentBulletSpeed();	

	++shotsFired;
	bulletsFire(bullets, start, forward, speed, 0.6f, 5.0f);
	// BONUS: Play sound effect on firing
	sound::playSFX("assets/shoot.ogg", 1.0f);

}

// resumeFromMenu(): Resume game from menu state
void Game::resumeFromMenu() {
	resetRound();
	glutPostRedisplay();
}

// isESVMainActive(): Check if the ESV camera is the current main render camera
bool Game::isESVMainActive() const {
	return (cams.renderCam == &cams.cameraESV);
}

// updateArcballCamera(): Update the ESV camera based on arcball parameters
void Game::updateArcballCamera() {
	auto& cam = cams.cameraESV;
	float orbitR = cam.getOrbitRadius();
	float orbitTh = cam.getOrbitTheta();
	float orbitPh = cam.getOrbitPhi();
	cam.setOrbit(orbitR, orbitTh, orbitPh);
	cam.updateOrbitParam();
}

// handleMouseButton(): Handle mouse button events for arcball control
// Left-drag to rotate, right-drag to zoom
void Game::handleMouseButton(int button, int state, int x, int y) {
	if (gameState == GameState::RoundOver) return;
	if (!isESVMainActive()) return;

	if (button == GLUT_LEFT_BUTTON) {
		arcball.rotating = (state == GLUT_DOWN);
		arcball.lastX = x;
		arcball.lastY = y;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		arcball.zooming = (state == GLUT_DOWN);
		arcball.lastX = x;
		arcball.lastY = y;
	}
}

// handleMouseMotion(): Handle mouse motion events for arcball control
// Updates camera orbit angles or zoom based on mouse movement
void Game::handleMouseMotion(int x, int y) {
	if (!isESVMainActive()) return;

	int dx = x - arcball.lastX;
	int dy = y - arcball.lastY;
	arcball.lastX = x;
	arcball.lastY = y;

	auto& cam = cams.cameraESV;

	if (arcball.rotating) {
		float newTheta = cam.getOrbitTheta() + (dx * arcball.rotSens);
		float newPhi = cam.getOrbitPhi() + (dy * arcball.rotSens);
		cam.setOrbit(cam.getOrbitRadius(), newTheta, newPhi);
		const float eps = 0.02f;
		if (cam.getOrbitPhi() < eps) {
			cam.setOrbitPhi(eps);
		}
		else if (cam.getOrbitPhi() > (3.14f - eps)) {
			cam.setOrbitPhi(3.14f - eps);
		}
		updateArcballCamera();
		glutPostRedisplay();
	}
	else if (arcball.zooming) {
		float newR = cam.getOrbitRadius();
		newR += dy * arcball.zoomSens * 0.5f;
		if (newR < arcball.minR) newR = arcball.minR;
		if (newR > arcball.maxR) newR = arcball.maxR;
		cam.setOrbitRadius(newR);
		updateArcballCamera();
	}
}