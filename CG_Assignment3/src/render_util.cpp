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


void DrawCube(RenderMode mode, float size = 1.0f) {
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

void DrawSphere(RenderMode mode, float radius = 1.0f) {
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