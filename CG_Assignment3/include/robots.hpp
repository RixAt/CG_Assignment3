// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____   ___  ____   ___ _____ ____  
// |  _ \ / _ \| __ ) / _ \_   _/ ___| 
// | |_) | | | |  _ \| | | || | \___ \ 
// |  _ <| |_| | |_) | |_| || |  ___) |
// |_| \_\\___/|____/ \___/ |_| |____/ 
// 
// ====================================================================
// File: robots.hpp
// Description: Header file for Robot class, defines the structure,
// behavior, and rendering of robot enemies in the game.
// Robots are composed of simple geometric shapes.
// ====================================================================
#ifndef ROBOTS_HPP
#define ROBOTS_HPP

#include <GL/glut.h>
#include "gameobject.hpp"
#include "utilities.hpp"
#include "render_util.hpp"


// Class: Robot
// Represents a robot enemy in the game. Inherits from basic GameObject.
// Each robot has position, bounding radius, animation phase, color, and alive state.
// Robots orbit around a center point while bobbing up and down.
// They have simple arm and leg swinging animations based on their animation phase.
class Robot : public GameObject {
private:
	float radius;		// Bounding sphere radius for collision detection
	float animPhase;    // Animation phase
	Vector3 color;      // Base color for rendering
	bool alive;         // Is the robot alive in the scene?

	// Orbiting parameters
	Vector3 orbitCenter = Vector3(0.0f, 0.0f, 0.0f);
	float orbitRadius = 12.0f;
	float orbitAngle = 0.0f; // Current angle around orbit center
	float orbitSpeed = 0.6f; 

	// Bobbing parameters
	float bobAmp = 0.4f;
	float bobFreq = 1.5f;

	// Animation parameters
	float stepFreq = 2.2f;
	float armSwingDeg = 30.0f;
	float legSwingDeg = 25.0f;

	// Body part helpers
	void drawTorso(RenderMode mode) const;
	void drawHead(RenderMode mode)  const;
	void drawArm(RenderMode mode, bool isLeftSide) const;
	void drawLeg(RenderMode mode, bool isLeftSide) const;
public:
	// Ctors
	// Default ctor
	Robot();
	// Parameterized ctor
	Robot(const Vector3& startPos, float r);

	// Update robots position and animation over time
	void update(float dt) override;

	// Draw robot model at current position with animation in specified render mode
	void draw(RenderMode mode) const override;

	// Draw wireframe robot collider for debugging
	void drawColliderDebug() const;

	// Check if a bullet at bulletPos with bulletRadius hits this robot
	bool checkHit(const Vector3& bulletPos, float bulletRadius) const;

	// Marks the robot as killed
	void kill();
	// Returns true if robot is alive
	bool isAlive() const { return alive; }

	// Accessor for radius
	float getRadius() const { return radius; }

};


#endif // !ROBOTS_HPP
