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
// Description: 
// 
// ====================================================================

#include "game.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

Game::Game()
	: g_renderMode(RenderMode::Solid),
	showAxes(true),
	showColliders(false),
	motionEnabled(true)
{
	
}

// Initialize game state
void Game::init() {
	srand(time(nullptr));
	robots.clear();

	// Spawns ten robots at random XY positions on ground plane
	for (int i = 0; i < 10; ++i) {
		float x = (std::rand() % 200 - 100);
		float z = (std::rand() % 200 - 100);
		Vector3 spawnPos(x, 0.0f, z);

		robots.push_back(new Robot(spawnPos, 12.0f));
	}

	bullets.reserve(maxBullets);

	cams.setRenderFPV();
	cams.controlCam = &cams.cameraFPV;
}

// Update game state
// dt in seconds
void Game::update(float dt) {
	if (gameState == GameState::RoundOver) return;
	if (motionEnabled) {
		for (auto& r : robots) {
			r->update(dt);
		}
	}
	effectsUpdate(impacts, dt);
	
	timeRemaining = std::max(0.0f, timeRemaining - dt);
	if (timeRemaining <= 0.0f) {
		endRound(false);
	}

	bulletsUpdate(bullets, dt, robots, robotsKilled, shotsHit, score, 3.0f, 100, &impacts);
}

// Draw the game scene
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

void Game::drawCameraDebugLines(const Camera* exclude, float aspect) const {
	if (showCameraFrustums) {
		if (exclude != &cams.cameraFPV) {
			DrawCameraFrustum(cams.cameraFPV, aspect, 0.5f, Vector3(1.0f, 0.5f, 0.0f));
		}
		if (exclude != &cams.cameraESV) {
			DrawCameraFrustum(cams.cameraESV, aspect, 1.0f, Vector3(0.2f, 0.7f, 1.0f));
		}
		if (exclude != &cams.cameraFree) {
			DrawCameraFrustum(cams.cameraFree, aspect, 0.5f, Vector3(0.5f, 1.0f, 0.5f));
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
			DrawCameraMarker(cams.cameraFree, 2.0f, Vector3(0.5f, 1.0f, 0.5f));
		}
	}
}


Game::ViewportsLayout Game::computeViewports(int winW, int winH) const {
	ViewportsLayout allViewports{};

	const int hudH = std::max(1, winH / 8);
	const int mainH = std::max(1, winH - hudH);
	const int mainW = winW;

	allViewports.vpHUD.x = 0; allViewports.vpHUD.width = winW;
	allViewports.vpHUD.height = hudH;
	allViewports.vpHUD.y = winH - hudH;

	allViewports.vpMain.x = 0;
	allViewports.vpMain.y = 0;
	allViewports.vpMain.width = mainW;
	allViewports.vpMain.height = mainH;

	const int insetW = std::max(120, mainW / 4);
	const int insetH = std::max(90, mainH / 4);
	allViewports.vpInset.width = insetW;
	allViewports.vpInset.height = insetH;
	allViewports.vpInset.x = allViewports.vpMain.x + allViewports.vpMain.width - insetW - 10;
	allViewports.vpInset.y = allViewports.vpMain.y + allViewports.vpMain.height - insetH - 10;

	return allViewports;
}

void Game::drawMainViewport(const Viewport& vp) const {
	SetViewport(vp);
	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
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

void Game::drawInsetViewport(const Viewport& vp) const {
	SetViewport(vp);
	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	const Camera* insetCam = (cams.renderCam == &cams.cameraFPV) ? &cams.cameraESV
							: (cams.renderCam == &cams.cameraESV) ? &cams.cameraFPV
							: &cams.cameraESV;
	
	insetCam->applyView(vp.width, vp.height);
	drawWorld();
	bulletsDraw(bullets, g_renderMode);
	effectsDrawImpacts(impacts);

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

	const int pad = 12;
	DrawText2D(pad, vp.height - 20, "Robot Hunter");
	DrawText2D(pad, vp.height - 40, ("Score: " + std::to_string(score)).c_str());
	DrawText2D(pad, vp.height - 60, ("Robots Killed: " + std::to_string(robotsKilled)).c_str());
	DrawText2D(pad, vp.height - 80, ("Remaining Time: " + std::to_string((int)std::ceilf(timeRemaining)) + " / 30").c_str());
	DrawText2D(12*pad, vp.height - 20, ("Bullet Speed: " + std::string(bulletSpeedLabel())).c_str());
	DrawText2D(12*pad, vp.height - 40, ("Shots: " + std::to_string(shotsHit) + "/" + std::to_string(shotsFired)).c_str());
	DrawText2D(12*pad, vp.height - 60, ("Accuracy: " + std::to_string((int)std::round(accuracyPercentage())) + "%").c_str());

	if (isRoundOver()) DrawText2D(vp.width / 2 - 60, vp.height / 2, "ROUND OVER! (Press R to Reset)", GLUT_BITMAP_HELVETICA_18);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void Game::handleKey(unsigned char key) {
	if (gameState == GameState::RoundOver) {
		if (key == 'r' || key == 'R') {
			resetRound();
			glutPostRedisplay();
			return;
		} 
		if (key == 27) exit(0);
		return;
	}
	
	
	switch (key) {
	case 27: // ESC key
		exit(0);
		break;
	case 'w':
	case 'W':
		g_renderMode = RenderMode::Wireframe;
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		g_renderMode = RenderMode::Solid;
		glutPostRedisplay();
		break;
	case 'v':
	case 'V':
		g_renderMode = RenderMode::Vertices;
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		showAxes = !showAxes;
		glutPostRedisplay();
		break;
	case 'c':
	case 'C':
		showColliders = !showColliders;
		glutPostRedisplay();
		break;
	case 'i':
	case 'I':
		// Print instructions
		break;
	case '3':
		cams.activateFreeCam();
		break;
	case 'm':
	case 'M':
		motionEnabled = !motionEnabled;
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
		cams.controlCam->elevate(cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		cams.controlCam->elevate(-cams.controlCam->moveSpeed);
		glutPostRedisplay();
		break;
	case 'r':
	case 'R':
		resetRound();
		glutPostRedisplay();
		break;
	case ' ':
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

void Game::handleSpecialKey(int key) {
	if (gameState == GameState::RoundOver) {
		return;
	}

	switch (key) {
	case GLUT_KEY_F1:
		// Toggle fullscreen
		toggleFullscreen();
		break;
	case GLUT_KEY_F2:
		// Toggle view mode
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

void Game::endRound(bool success) {
	gameState = GameState::RoundOver;
	missionSuccess = success;
}

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

void Game::onBulletFired() {
	if (gameState == GameState::RoundOver) return;
	//shotsFired++;
}

void Game::onBulletMiss() {
	if (gameState == GameState::RoundOver) return;
}

void Game::fireBulletFromCamera(const Camera& cam) {
	if (gameState == GameState::RoundOver) return;
	if (!cams.controlCam) return;

	Vector3 eye, right, up, forward;
	cam.getEyeBasis(eye, right, up, forward);

	const Vector3 start = eye + forward * 1.5f;
	const float speed = currentBulletSpeed();	

	++shotsFired;
	bulletsFire(bullets, start, forward, speed, 0.6f, 2.0f);
}

void Game::resumeFromMenu() {
	resetRound();
	glutPostRedisplay();
}

bool Game::isESVMainAcive() const {
	return (cams.renderCam == &cams.cameraESV);
}

void Game::updateArcballCamera() {
	auto& cam = cams.cameraESV;
	float orbitR = cam.getOrbitRadius();
	float orbitTh = cam.getOrbitTheta();
	float orbitPh = cam.getOrbitPhi();
	cam.setOrbit(orbitR, orbitTh, orbitPh);
	cam.updateOrbitParam();
}

void Game::handleMouseButton(int button, int state, int x, int y) {
	if (gameState == GameState::RoundOver) return;
	if (!isESVMainAcive()) return;

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

void Game::handleMouseMotion(int x, int y) {
	if (!isESVMainAcive()) return;

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
		float newR = cam.getOrbitRadius() + (1.0f - dy * arcball.zoomSens);
		if (newR < arcball.minR) newR = arcball.minR;
		if (newR > arcball.maxR) newR = arcball.maxR;
		cam.setOrbitRadius(newR);
		updateArcballCamera();
	}
}