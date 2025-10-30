// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//   ____    _    __  __ _____ ____      _    
//  / ___|  / \  |  \/  | ____|  _ \    / \   
// | |     / _ \ | |\/| |  _| | |_) |  / _ \  
// | |___ / ___ \| |  | | |___|  _ <  / ___ \ 
//  \____/_/   \_\_|  |_|_____|_| \_\/_/   \_\
// 
// ====================================================================
// File: camera.cpp
// Description: 
// 
// ====================================================================

#include "camera.hpp"

// Camera constructor with default parameters
Camera::Camera()
	: position(0.0f, 20.0f, 40.0f),
	  yaw(0.0f),
	  pitch(0.0f),
	  roll(0.0f),
	  mode(CameraMode::EntireScene),
	  fovY(60.0f),
	  zNear(1.0f),
	  zFar(1000.0f),
	  moveSpeed(1.0f),
	  turnSpeed(0.05f),
	  fpvEyeHeight(2.0f),
	  fpvAnchor(nullptr)
{
}

// Apply the camera view and projection transformations
void Camera::applyView(int winW, int winH) const {
	if (winW <= 0) winW = 1;
	if (winH <= 0) winH = 1;

	// Set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovY, (float)winW / (float)winH, zNear, zFar);

	// Set modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Compute look direction
	const float cy = std::cosf(yaw);
	const float sy = std::sinf(yaw);
	const float cp = std::cosf(pitch);
	const float sp = std::sinf(pitch);

	Vector3 forward(sy * cp, -sp, cy * cp);
	Vector3 eye = position;

	Vector3 target = eye + forward;

	gluLookAt(
		eye.x, eye.y, eye.z,
		target.x, target.y, target.z,
		0.0f, 1.0f, 0.0f
	);
}

// Move the camera forward in direction facing in XZ plane
void Camera::moveForward(float amount) {
	position.x += std::sinf(yaw) * amount;
	position.z += std::cosf(yaw) * amount;
}

// Move the camera backward in direction facing in XZ plane
void Camera::moveBackward(float amount) {
	moveForward(-amount);
}

// Move the camera right perpendicular to facing direction in XZ plane
void Camera::moveRight(float amount) {
	position.x += std::cosf(yaw) * amount;
	position.z += std::sinf(yaw) * amount;
}

// Move the camera left perpendicular to facing direction in XZ plane
void Camera::moveLeft(float amount) {
	moveRight(-amount);
}

void Camera::elevate(float amount) {
	position.y += amount;
}

void Camera::turnLeft(float amount) {
	yaw -= amount;
}
void Camera::turnRight(float amount) {
	turnLeft(-amount);
}
