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
// File: camera.hpp
// Description: 
// 
// ====================================================================

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glut.h>
#include "utilities.hpp"

enum class CameraMode {
	FirstPerson,
	EntireScene,
	FreeCam
};

class Camera {
public:
	Vector3 position;
	float yaw; // Rotation around the Y axis
	float pitch; // Rotation around the X axis, clamped to avoid flipping
	float roll; // Rotation around the Z axis

	CameraMode mode;
	float fovY; // Field of view in Y direction
	float zNear, zFar; // Near and far clipping planes
	float moveSpeed; // Movement speed, units per input step
	float turnSpeed; // Turn speed, radians per input step
	float fpvEyeHeight; // Height of the eye from ground in first person view

	const Vector3* fpvAnchor; // Pointer to the object the FPV camera is anchored to

	Camera();

	void applyView(int winW, int winH) const;

	void moveForward(float amount);
	void moveBackward(float amount);
	void moveLeft(float amount);
	void moveRight(float amount);
	void elevate(float amount);

	void turnLeft(float amount);
	void turnRight(float amount);
	void lookUp(float amount);
	void lookDown(float amount);

	void setMode(CameraMode newMode);
	void toggleMode();

	void setProjection(float fovYDegrees, float nearZ, float farZ);

	void setFPVAnchor(const Vector3* anchor);
	void setFPVEyeHeight(float height);

	Vector3 getPosition() const { return position; }
	float getYaw() const { return yaw; }
	float getPitch() const { return pitch; }

private:

};


#endif // !CAMERA_HPP
