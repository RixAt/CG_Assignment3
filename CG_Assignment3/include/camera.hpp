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
// Description: Header file for Camera class and CameraSystem
// 
// ====================================================================

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glut.h>
#include "utilities.hpp"
#include <algorithm>

// Enum: CameraMode
// Different modes the camera can operate in
enum class CameraMode {
	FirstPerson,
	EntireScene,
	FreeCam
};

// Class: Camera
// Represents a 3D camera with position, orientation, and projection
class Camera {
public:
	// Ctor
	Camera();

	// Apply the camera view and projection transformations
	void applyView(int winW, int winH) const;

	// Movement methods
	void moveForward(float amount);
	void moveBackward(float amount);
	void moveLeft(float amount);
	void moveRight(float amount);
	void elevate(float amount);

	void turnLeft(float amount);
	void turnRight(float amount);
	void lookUp(float amount);
	void lookDown(float amount);

	// Configure camera
	void setMode(CameraMode newMode);
	void toggleMode();

	void setProjection(float fovYDegrees, float nearZ, float farZ);
	void setFPVAnchor(const Vector3* anchor) { fpvAnchor = anchor; }
	void setFPVEyeHeight(float height) { fpvEyeHeight = height; };

	void setOrbitTarget(const Vector3& t) { orbitTarget = t; };
	void setOrbit(float r, float th, float ph);
	void updateOrbitParam();

	Vector3 getPosition() const { return position; }
	float getYaw() const { return yaw; }
	float getPitch() const { return pitch; }
	float getRoll() const { return roll; }
	void getEyeBasis(Vector3& eye,Vector3& right, Vector3& up, Vector3& forward) const;
	float getFovY() const { return fovY; }
	float getNearZ() const { return zNear; }
	float getFarZ() const { return zFar; }
	float getOrbitRadius() const { return orbitRadius; }
	float getOrbitTheta() const { return orbitTheta; }
	float getOrbitPhi() const { return orbitPhi; }

	void setPosition(const Vector3& pos) { position = pos; }
	void setYaw(float newYaw) { yaw = newYaw; }
	void setPitch(float newPitch) { pitch = clampPitch(newPitch); }
	void setRoll(float newRoll) { roll = newRoll; }
	void setFovY(float newFovY) { fovY = newFovY; }
	void setNearZ(float newNearZ) { zNear = newNearZ; }
	void setFarZ(float newFarZ) { zFar = newFarZ; }
	void setOrbitRadius(float r) { orbitRadius = std::max(1.0f, r); }
	void setOrbitTheta(float th) { orbitTheta = th; }
	void setOrbitPhi(float ph) { orbitPhi = ph; }

	// (Public) Move speeds
	float moveSpeed; // Movement speed, units per input step
	float turnSpeed; // Turn speed, radians per input step

private:
	// Camera pose
	Vector3 position;
	float yaw; // Rotation around the Y axis
	float pitch; // Rotation around the X axis, clamped to avoid flipping
	float roll; // Rotation around the Z axis

	// Camera projection
	float fovY; // Field of view in Y direction
	float zNear, zFar; // Near and far clipping planes

	// Camera mode
	CameraMode mode;

	//FPV specific
	float fpvEyeHeight; // Height of the eye from ground in first person view
	const Vector3* fpvAnchor; // Pointer to the object the FPV camera is anchored to

	// Clamp pitch to avoid flipping
	static float clampPitch(float pitchValue);
	// Check if camera is locked (not movable)
	bool isLocked() const { return mode == CameraMode::EntireScene; }

	// Arcball
	Vector3 orbitTarget{ 0,0,0 };
	float orbitRadius = 200.0f;
	float orbitTheta = -2.356f;
	float orbitPhi = 0.8f;

};


// Class: CameraSystem
// Manages multiple cameras and switching between them
class CameraSystem {
public:
	// Cameras available
	Camera cameraFPV;
	Camera cameraESV;
	Camera cameraFree;

	// Active cameras
	Camera* renderCam = nullptr;  // Current camera used for rendering
	Camera* controlCam = nullptr; // Current camera used for user control

	// Ctor
	CameraSystem() {
		// Initialize cameras
		cameraFPV.setMode(CameraMode::FirstPerson);
		cameraFPV.setFPVEyeHeight(11.1f);
		cameraFPV.setPosition(Vector3(0, 2, 10));
		cameraFPV.setProjection(60.0f, 0.1f, 1000.0f);

		cameraESV.setMode(CameraMode::EntireScene);
		cameraESV.setPosition(Vector3(0.0f, 100.0f, 160.0f));
		//cameraESV.setYaw(0.0f);
		//cameraESV.setPitch(-0.65f);
		cameraESV.setProjection(60.0f, 0.1f, 1000.0f);
		cameraESV.setOrbit(200.0f, -2.356f, 0.8f);
		cameraESV.setOrbitTarget(Vector3(0.0f, 0.0f, 0.0f));
		cameraESV.updateOrbitParam();

		cameraFree.setMode(CameraMode::FreeCam);
		cameraFree.setPosition(Vector3(0.0f, 20.0f, 40.0f));
		cameraFree.setProjection(60.0f, 0.1f, 1000.0f);
	}

	// Toggle between FPV and ESV for rendering
	void toggleFPV_ESV() {
		renderCam = (renderCam == &cameraFPV) ? &cameraESV : &cameraFPV;
		// Note: controlCam remains unchanged
	}

	// Set rendering camera
	void setRenderFPV() { renderCam = &cameraFPV; };
	void setRenderESV() { renderCam = &cameraESV; };
	void setRenderFreeCam() { renderCam = &cameraFree; };

	// Set control camera
	void activateFreeCam() {
		renderCam = &cameraFree;
		controlCam = &cameraFree;
	}


};


#endif // !CAMERA_HPP
