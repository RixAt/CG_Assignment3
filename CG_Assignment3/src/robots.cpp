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

// Body helpers

void Robot::drawHead(RenderMode mode) const {
	glPushMatrix();
		glTranslatef(0.0f, 6.0f, 0.0f);
		DrawSphere(mode, 2.0f);
	glPopMatrix();
}

void Robot::drawTorso(RenderMode mode) const {
	glPushMatrix();
		glScalef(5.0f, 8.0f, 3.0f);
		DrawCube(mode);
	glPopMatrix();
}

void Robot::drawArm(RenderMode mode, bool isLeftSide) const {
	float side = isLeftSide ? -1.0f : 1.0f;

	glPushMatrix();
		glTranslatef(3.0f * side, 3.0f, 0.0f);
		// Shoulder swing here during dancing?

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

void Robot::drawLeg(RenderMode mode, bool isLeftSide) const {
	float side = isLeftSide ? -1.0f : 1.0f;
	glPushMatrix();
		glTranslatef(1.5f * side, -4.0f, 0.0f);

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

// Update
void Robot::update(float dt) {
	
}

// Draw
void Robot::draw(RenderMode mode) const {
	if (!alive) return; // Skip drawing if not alive

	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);

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

		drawTorso(mode);
		drawHead(mode);
		drawArm(mode, true);
		drawArm(mode, false);
		drawLeg(mode, true);
		drawLeg(mode, false);

	glPopMatrix();
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