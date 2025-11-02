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

	cams.setRenderFPV();
	cams.controlCam = &cams.cameraFPV;
}

// Update game state
// dt in seconds
void Game::update(float dt) {
	if (!motionEnabled) return;

	for (auto& r : robots) {
		r->update(dt);
	}
}

// Draw the game scene
void Game::draw(int winW, int winH) const {
	// Draw game objects here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)winW / (float)winH, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cams.renderCam->applyView(winW, winH);

	DrawGround();

	for (const auto& r : robots) {
		r->draw(g_renderMode);

		if (showColliders) {
			r->drawColliderDebug();
		}
	}

	const float aspect = (winH > 0) ? (float)winW / (float)winH : 1.0f;

	if (cams.renderCam != &cams.cameraFPV) {
		DrawCameraFrustum(cams.cameraFPV, aspect, 0.5f, Vector3(1.0f, 0.5f, 0.0f));
		DrawCameraMarker(cams.cameraFPV, 1.5f, Vector3(1.0f, 0.5f, 0.0f));
	}
    if (cams.renderCam != &cams.cameraESV) {
		DrawCameraMarker(cams.cameraESV, 1.3f, Vector3(0.2f, 0.7f, 1.0f));
		DrawCameraFrustum(cams.cameraESV, aspect, 1.0f, Vector3(0.2f, 0.7f, 1.0f));
	}

	if (cams.renderCam != &cams.cameraFree) {
		DrawCameraFrustum(cams.cameraFree, aspect, 0.5f, Vector3(0.5f, 1.0f, 0.5f));
		DrawCameraMarker(cams.cameraFree, 1.5f, Vector3(0.5f, 1.0f, 0.5f));
	}

	glutSwapBuffers();
}

void Game::handleKey(unsigned char key) {
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
	default:
		break;
	}
};

void Game::handleSpecialKey(int key) {
	switch (key) {
	case GLUT_KEY_F1:
		// Toggle fullscreen
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