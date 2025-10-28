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
// ====================================================================
// File: robots.hpp
// Description: Header file for Robot class
// 
// ====================================================================
#ifndef ROBOTS_HPP
#define ROBOTS_HPP

#include <GL/glut.h>
#include "utilities.hpp"

class Robot {
public:
	Robot();

	// Core behavior
	void update();
	void draw();

	// Accessors
	const Vector3& getPosition() const { return position; }

	// Setters
	void setPosition(const Vector3& pos) { position = pos; }


private: 
	Vector3 position;
	float radius;
	float rotation;

	Vector3 color;
};


#endif // !ROBOTS_HPP
