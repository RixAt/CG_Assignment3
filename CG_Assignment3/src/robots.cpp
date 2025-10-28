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
// Description: Header file for Robot class
// 
// ====================================================================

#include "robots.hpp"

Robot::Robot()
	: GameObject(Vector3(0.0f, 0.0f, 0.0f)),
	radius(10.0f),
	animPhase(0.0f),
	color(0.8f, 0.1f, 0.1f),
	alive(true)
{

}

Robot::Robot(const Vector3& startPos, float r)
	: GameObject(startPos),
	radius(r),
	animPhase(0.0f),
	color(0.8f, 0.1f, 0.1f),
	alive(true)
{

}

// Update
void Robot::update(float dt) {
	
}

// Draw
void Robot::draw() const {
	if (!alive) return; // Skip drawing if not alive

	// This is where the robot object will be drawn
}

void Robot::drawColliderDebug() const {
	if (!alive) return; // Skip drawing if not alive
}

bool Robot::checkHit(const Vector3 &bulletPos, float bulletRadius) const{
	if (!alive) return false; // If not alive, robot was not hit
}

void Robot::kill() {
	alive = false;
	active = false;
}