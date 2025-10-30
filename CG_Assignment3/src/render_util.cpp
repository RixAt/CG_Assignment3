// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____  _____ _   _ ____  _____ ____              
// |  _ \| ____| \ | |  _ \| ____|  _ \             
// | |_) |  _| |  \| | | | |  _| | |_) |            
// |  _ <| |___| |\  | |_| | |___|  _ <             
// |_| \_\_____|_|_\_|____/|_____|_|_\_\_____ ____  
// | | | |_   _|_ _| |   |_ _|_   _|_ _| ____/ ___| 
// | | | | | |  | || |    | |  | |  | ||  _| \___ \ 
// | |_| | | |  | || |___ | |  | |  | || |___ ___) |
//  \___/  |_| |___|_____|___| |_| |___|_____|____/ 
// 
// ====================================================================
// File: render_util.cpp
// Description: 
// 
// ====================================================================

#include "render_util.hpp"


void DrawCube(RenderMode mode, float size) {
	glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT | GL_POINT_BIT | GL_ENABLE_BIT | GL_LINE_BIT);

	switch (mode) {
	case RenderMode::Solid:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutSolidCube(size);
		break;
	case RenderMode::Wireframe:
		glutWireCube(size);
		break;
	case RenderMode::Vertices:
		//glPointSize(3.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glutSolidCube(size);
		break;
	}

	glPopAttrib();
}

void DrawSphere(RenderMode mode, float radius) {
	glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT | GL_POINT_BIT | GL_ENABLE_BIT | GL_LINE_BIT);

	switch (mode) {
	case RenderMode::Solid:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutSolidSphere(radius, 16, 16);
		break;
	case RenderMode::Wireframe:
		glutWireSphere(radius, 16, 16);
		break;
	case RenderMode::Vertices:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glutSolidSphere(radius, 16, 16);
		break;
	}

	glPopAttrib();
}

void DrawPlane(RenderMode mode, float width, float depth, const Vector3& color) {
	glColor3f(color.x, color.y, color.z);
	float w = width / 2.0f;
	float d = depth / 2.0f;

	glBegin(GL_QUADS);
		glVertex3f(-w, 0.0f, -d);
		glVertex3f(w, 0.0f, -d);
		glVertex3f(w, 0.0f, d);
		glVertex3f(-w, 0.0f, d);
	glEnd();
}

void DrawGround(float size, float spacing) {
	// Solid ground quad
	DrawPlane(RenderMode::Solid, size, size, Vector3(0.2f, 0.2f, 0.2f));

	// Adding grid lines to help visualize scale
	glColor3f(0.3f, 0.3f, 0.3f);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for (float i = -size / 2.0f; i <= size / 2.0f; i += spacing) {
		// lines parallel to Z
		glVertex3f(i, 0.01f, -size / 2.0f);
		glVertex3f(i, 0.01f, size / 2.0f);
		// lines parallel to X
		glVertex3f(-size / 2.0f, 0.01f, i);
		glVertex3f(size / 2.0f, 0.01f, i);
	}
	glEnd();
}

void DrawCameraMarker(const Camera& cam) {
	glPushMatrix();
		glTranslatef(cam.position.x, cam.position.y, cam.position.z);
		glRotatef(-cam.yaw * 180.0f / 3.14159f, 0.0f, 1.0f, 0.0f);
		glRotatef(-cam.pitch * 180.0f / 3.14159f, 1.0f, 0.0f, 0.0f);
		glColor3f(1.0f, 0.8f, 0.0f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.5f, 0.0f); glVertex3f(-0.2f, 0.0f, 0.3f); glVertex3f(0.2f, 0.0f, 0.3f);
			glVertex3f(0.0f, 0.5f, 0.0f); glVertex3f(0.2f, 0.0f, 0.3f);  glVertex3f(0.2f, 0.0f, -0.3f);
			glVertex3f(0.0f, 0.5f, 0.0f); glVertex3f(0.2f, 0.0f, -0.3f); glVertex3f(-0.2f, 0.0f, -0.3f);
			glVertex3f(0.0f, 0.5f, 0.0f); glVertex3f(-0.2f, 0.0f, -0.3f); glVertex3f(-0.2f, 0.0f, 0.3f);
		glEnd();
		glColor3f(1.0f, 0.5f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(-0.1f, 0.0f, 0.2f);
			glVertex3f(0.1f, 0.0f, 0.2f); 
			glVertex3f(0.1f, 0.0f, -0.2f); 
			glVertex3f(-0.1f, 0.0f, -0.2f);
		glEnd();

	glPopMatrix();
}