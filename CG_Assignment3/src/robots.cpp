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
// File: robots.cpp
// Description: File for Robot class, implements its drawing,
// animation, and collision logic. Each robot is a simple humanoid
// made of cubes and spheres, andimated with bobbing and arm/leg
// swinging.
// ====================================================================

#include "robots.hpp"
#include <algorithm>

// Radius used for robot collision detection
static const float ROBOT_COLLIDER_RADIUS = 8.0f;
static const float ROBOT_MODEL_Y_OFFSET = 11.5f; // Offset to position model above ground
// Constant for 2*PI
static const float TWO_PI = 6.28318530718f;
static const float PI = 3.14159265359f;

// Ctors
// Default constructor places robot at origin with default color, idle animation phase
Robot::Robot()
	: GameObject(Vector3(0.0f, 0.0f, 0.0f)),
	radius(ROBOT_COLLIDER_RADIUS),
	animPhase(0.0f),
	color(0.8f, 0.1f, 0.1f),
	alive(true),
	danceEnabled(true)
{
	orbitCenter = position;
	orbitRadius = 12.0f;
	orbitAngle = 0.0f;
	orbitSpeed = 0.6f;

	bobAmp = 0.4f;
	bobFreq = 1.5f;

	stepFreq = 2.2f;
	armSwingDeg = 30.0f;
	legSwingDeg = 25.0f;

	heading = fmodf((position.x * 0.13f + position.z * 0.47f), 6.28318f);
}

// Parameterized constructor places robot at given position with given radius
// Orbital position is randomized based on startPos for variety
Robot::Robot(const Vector3& startPos, float r)
	: GameObject(startPos),
	radius(r),
	animPhase(0.0f),
	color(0.8f, 0.1f, 0.1f),
	alive(true),
	danceEnabled(true)
{
	orbitCenter = startPos;
	orbitRadius = std::max(8.0f, r);
	orbitAngle = (startPos.x + startPos.z) * 0.07f;
	orbitSpeed = (((int)std::fabs(startPos.x + startPos.z)) % 2 ? 1.f : -1.f) * 0.6f;

	bobAmp = 0.4f;
	bobFreq = 1.5f;

	stepFreq = 2.2f;
	armSwingDeg = 30.0f;
	legSwingDeg = 25.0f;

	heading = fmodf((startPos.x * 0.13f + startPos.z * 0.47f), 6.28318f);
}

// Body helpers
// Each part draws a specific body component with transforms relative to the robot origin

// drawHead(): Draws the robot's head as a sphere
void Robot::drawHead(RenderMode mode) const {
	glPushMatrix();
		glTranslatef(0.0f, 6.0f, 0.0f);
		DrawSphere(mode, 2.0f);
	glPopMatrix();
}

// drawTorso(): Draws the robot's torso as a scaled cube
void Robot::drawTorso(RenderMode mode) const {
	glPushMatrix();
		glScalef(5.0f, 8.0f, 3.0f);
		DrawCube(mode);
	glPopMatrix();
}

// drawArm(): Draws one arm (left or right) with swinging animation
// Each arm consists of upper and lower segments
void Robot::drawArm(RenderMode mode, bool isLeftSide) const {
	float side = isLeftSide ? -1.0f : 1.0f;
	const float phase = isLeftSide ? 0.0f : PI;
	const float swing = armSwingDeg * std::sinf(stepFreq * animPhase * TWO_PI + phase);
	

	glPushMatrix();
		glTranslatef(3.0f * side, 3.0f, 0.0f);
		glRotatef(swing, 1.0f, 0.0f, 0.0f);

		// Upper arm
		glPushMatrix();
			glTranslatef(0.0f, -2.0f, 0.0f);
			glScalef(2.0f, 4.0f, 2.0f);
			DrawCube(mode);
		glPopMatrix();

		// Lower arm
		glPushMatrix();
			glTranslatef(0.0f, -4.0f, 0.0f);
			glScalef(1.5f, 3.5f, 1.5f);
			DrawCube(mode);
		glPopMatrix();

	glPopMatrix();
}

// drawLeg(): Draws one leg (left or right) with swinging animation
// Each leg consists of upper and lower segments
void Robot::drawLeg(RenderMode mode, bool isLeftSide) const {
	float side = isLeftSide ? -1.0f : 1.0f;
	const float phase = isLeftSide ? PI : 0.0f ;
	const float swing = legSwingDeg * std::sinf(stepFreq * animPhase * TWO_PI + phase);

	glPushMatrix();
		glTranslatef(1.5f * side, -4.0f, 0.0f);
		glRotatef(swing, 1.0f, 0.0f, 0.0f);

		// Upper leg
		glPushMatrix();
			glTranslatef(0.0f, -3.0f, 0.0f);
			glScalef(2.0f, 6.0f, 2.0f);
			DrawCube(mode);
		glPopMatrix();
		
		// Lower leg
		glPushMatrix();
			glTranslatef(0.0f, -6.0f, 0.0f);
			glScalef(1.5f, 4.0f, 1.5f);
			DrawCube(mode);
		glPopMatrix();

	glPopMatrix();
}

// Update(): Updates robot position and animation phase
// Robot orbits around its center point while bobbing up and down
// Animation phase is incremented for arm/leg swinging
void Robot::update(float dt) {
	if (!alive) return;

	// BONUS: Simple wandering behavior
	static const float ARENA_HALF = 100.0f;   // Ground boundary
	static const float TURN_JITTER = 2.5f;     // Random turning (radians/sec)
	static const float BASE_SPEED_MIN = 4.0f;   // Minimum walking speed
	static const float BASE_SPEED_MAX = 12.0f;   // Maximum walking speed

	// Handle pause timer (occasional idle)
	if (pauseTimer > 0.0f) {
		pauseTimer -= dt;
		// Gentle idle bob while paused
		animPhase += dt * 0.5f;
		position.y = 0.2f * std::sinf(animPhase * 6.28318f);
		return;
	}

	// Occasionally choose a new random speed
	if (rand() % 250 == 0) {
		walkSpeed = BASE_SPEED_MIN + (rand() % (int)(BASE_SPEED_MAX - BASE_SPEED_MIN + 1));
	}

	// Occasionally pause for 1 second
	if (rand() % 500 == 0) {
		pauseTimer = 1.0f;
		return;
	}

	// Small random heading jitter for unique, non-repeating paths
	float jitter = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * TURN_JITTER;
	heading += jitter * dt;

	// Move forward in facing direction
	position.x += std::sinf(heading) * walkSpeed * dt;
	position.z += std::cosf(heading) * walkSpeed * dt;

	// Bounce off platform edges to stay on ground
	if (position.x > ARENA_HALF) { position.x = ARENA_HALF; heading += PI * 0.75f; }
	if (position.x < -ARENA_HALF) { position.x = -ARENA_HALF; heading -= PI * 0.75f; }
	if (position.z > ARENA_HALF) { position.z = ARENA_HALF; heading += PI * 0.75f; }
	if (position.z < -ARENA_HALF) { position.z = -ARENA_HALF; heading -= PI * 0.75f; }

	// Gentle vertical bobbing as they walk
	animPhase += dt;
	const float gaitScale = std::max(0.2f, walkSpeed / 6.0f);
	position.y = bobAmp * gaitScale * std::sinf(bobFreq * animPhase * TWO_PI);
	float t = animPhase;
	/*armSwingDeg = 40.0f * std::sinf(6.0f * t);
	legSwingDeg = 30.0f * std::sinf(6.0f * t + PI);*/
	torsoTwistDeg = 15.0f * std::sinf(3.0f * t);
	headNodDeg = 10.0f * std::sinf(4.0f * t + 1.2f);

}

// Draw(): Draws the robot model at its current position and orientation
// Applies renderMode and delegates to body part helpers
void Robot::draw(RenderMode mode) const {
	if (!alive) return; // Skip drawing if not alive

	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glTranslatef(0.0f, ROBOT_MODEL_Y_OFFSET, 0.0f);


		switch (mode) {
		case RenderMode::Wireframe:
			glColor3f(1.0f, 1.0f, 1.0f);
			glLineWidth(1.0f);
			break;
		case RenderMode::Solid:
			glColor3f(color.x, color.y, color.z);
			break;
		case RenderMode::Vertices:
			glColor3f(1.0f, 1.0f, 1.0f);
			glPointSize(3.0f);
			break;
		}

		glRotatef(heading * 180.0f / PI, 0, 1, 0);

		glPushMatrix();
		glRotatef(torsoTwistDeg, 0.0f, 1.0f, 0.0f);
		drawTorso(mode);

		glPushMatrix();
			//glTranslatef(0, 6, 0);
			glRotatef(headNodDeg, 1.0f, 0.0f, 0.0f);
			drawHead(mode);
		glPopMatrix();

		drawArm(mode, true);
		drawArm(mode, false);
		drawLeg(mode, true);
		drawLeg(mode, false);
		glPopMatrix();

	glPopMatrix();
}

// drawColliderDebug(): Draws the robot's collision sphere for debugging
// Visible when debug collider rendering is enabled
void Robot::drawColliderDebug() const {
	if (!alive) return; // Skip drawing if not alive
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glTranslatef(0.0f, ROBOT_MODEL_Y_OFFSET, 0.0f);
		glColor3f(0.0f, 1.0f, 1.0f);
		glutWireSphere(radius, 16, 16);
	glPopMatrix();
}

// checkHit(): Checks if a bullet at bulletPos with bulletRadius hits the robot
// Returns true if hit detected, false otherwise
// Uses sphere-sphere collision detection
bool Robot::checkHit(const Vector3 &bulletPos, float bulletRadius) const{
	if (!alive) return false; // If not alive, robot was not hit

	Vector3 c = position + Vector3(0.0f, ROBOT_MODEL_Y_OFFSET, 0.0f);
	Vector3 d = bulletPos - c;
	float rr = radius + bulletRadius;

	return (d.x * d.x + d.y * d.y + d.z * d.z) <= (rr * rr);
}

// kill(): Marks the robot as dead and deactivates it
void Robot::kill() {
	alive = false;
	active = false;
}