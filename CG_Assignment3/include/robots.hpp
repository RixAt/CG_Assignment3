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
#ifndef ROBOTS_HPP
#define ROBOTS_HPP

#include <GL/glut.h>
#include "gameobject.hpp"
#include "utilities.hpp"

class Robot : public GameObject {
private:
	float radius;		// Bounding sphere radius for collision detection
	float animPhase;    // Animation phase
	Vector3 color;      // Base color for rendering
	bool alive;         // Is the robot alive in the scene?
public:
	Robot();
	Robot(const Vector3& startPos, float r);

	void update(float dt) override;

	void draw() const override;

	void drawColliderDebug() const;

	bool checkHit(const Vector3& bulletPos, float bulletRadius) const;

	void kill();
	bool isAlive() const { return alive; }
	float getRadius() const { return radius; }

};


#endif // !ROBOTS_HPP
