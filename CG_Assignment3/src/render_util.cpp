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

void DrawAxes(float size) {
	glLineWidth(4.0f);
	glBegin(GL_LINES);
	// X red
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(size, 0, 0);
	// Y green
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, size, 0);
	// Z blue
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, size);
	glEnd();
}

void DrawCameraMarker(const Camera& cam, float size, const Vector3& color) {
	Vector3 eye, right, up, forward;
	cam.getEyeBasis(eye, right, up, forward);

	const Vector3 tip = eye + forward * (size * 1.5f);
	const Vector3 baseLeft = eye - right * (size * 0.6f) - up * (size * 0.3f);
	const Vector3 baseRight = eye + right * (size * 0.6f) - up * (size * 0.3f);
	const Vector3 baseUp = eye + up * (size * 0.6f);
	const Vector3 baseDown = eye - up * (size * 0.6f);

	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(1.5f);
	glColor3f(color.x, color.y, color.z);
	glBegin(GL_LINES);
	glVertex3f(baseLeft.x, baseLeft.y, baseLeft.z); glVertex3f(tip.x, tip.y, tip.z);
	glVertex3f(baseRight.x, baseRight.y, baseRight.z); glVertex3f(tip.x, tip.y, tip.z);
	glVertex3f(baseUp.x, baseUp.y, baseUp.z); glVertex3f(tip.x, tip.y, tip.z);
	glVertex3f(baseDown.x, baseDown.y, baseDown.z); glVertex3f(tip.x, tip.y, tip.z);

	glVertex3f(baseLeft.x, baseLeft.y, baseLeft.z); glVertex3f(baseRight.x, baseRight.y, baseRight.z);
	glVertex3f(baseUp.x, baseUp.y, baseUp.z); glVertex3f(baseDown.x, baseDown.y, baseDown.z);

	const float a = size * 0.8f;
	glVertex3f(eye.x, eye.y, eye.z); glVertex3f(eye.x + forward.x * a, eye.y + forward.y * a, eye.z + forward.z * a);
	glVertex3f(eye.x, eye.y, eye.z); glVertex3f(eye.x + right.x * a * 0.7f, eye.y + right.y * a * 0.7f, eye.z + right.z * a * 0.7f);
	glVertex3f(eye.x, eye.y, eye.z); glVertex3f(eye.x + up.x * a * 0.7f, eye.y + up.y * a * 0.7f, eye.z + up.z * a * 0.7f);

	glEnd();
	glPopAttrib();
	//glEnable(GL_LIGHTING);
}

void DrawCameraFrustum(const Camera& cam, float aspect, float scale, const Vector3& color) {
	Vector3 eye, right, up, forward;
	cam.getEyeBasis(eye, right, up, forward);
	const float fovYdeg = cam.getFovY();
	const float zn = cam.getNearZ() * scale;
	const float zf = cam.getFarZ() * scale;

	const float fovYrad = fovYdeg * 3.14159f / 180.0f;
	const float hNear = 2.0f * std::tanf(fovYrad / 2.0f) * zn; // height of near plane
	const float wNear = hNear * aspect; // width of near plane
	const float hFar = 2.0f * std::tanf(fovYrad / 2.0f) * zf; // height of far plane
	const float wFar = hFar * aspect; // width of far plane

	const Vector3 nc = eye + forward * zn; // near center
	const Vector3 fc = eye + forward * zf; // far center

	const Vector3 ntl = nc + (up * (hNear / 2.0f)) - (right * (wNear / 2.0f));
	const Vector3 ntr = nc + (up * (hNear / 2.0f)) + (right * (wNear / 2.0f));
	const Vector3 nbl = nc - (up * (hNear / 2.0f)) - (right * (wNear / 2.0f));
	const Vector3 nbr = nc - (up * (hNear / 2.0f)) + (right * (wNear / 2.0f));

	const Vector3 ftl = fc + (up * (hFar / 2.0f)) - (right * (wFar / 2.0f));
	const Vector3 ftr = fc + (up * (hFar / 2.0f)) + (right * (wFar / 2.0f));
	const Vector3 fbl = fc - (up * (hFar / 2.0f)) - (right * (wFar / 2.0f));
	const Vector3 fbr = fc - (up * (hFar / 2.0f)) + (right * (wFar / 2.0f));

	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_LIGHTING);
	glLineWidth(1.5f);
	glColor3f(color.x, color.y, color.z);
	glBegin(GL_LINES);
		// Near rectangle
		glVertex3f(ntl.x, ntl.y, ntl.z); glVertex3f(ntr.x, ntr.y, ntr.z); glVertex3f(ntr.x, ntr.y, ntr.z); glVertex3f(nbr.x, nbr.y, nbr.z);
		glVertex3f(nbr.x, nbr.y, nbr.z); glVertex3f(nbl.x, nbl.y, nbl.z); glVertex3f(nbl.x, nbl.y, nbl.z); glVertex3f(ntl.x, ntl.y, ntl.z);
		// Far rectangle
		glVertex3f(ftl.x, ftl.y, ftl.z); glVertex3f(ftr.x, ftr.y, ftr.z); glVertex3f(ftr.x, ftr.y, ftr.z); glVertex3f(fbr.x, fbr.y, fbr.z);
		glVertex3f(fbr.x, fbr.y, fbr.z); glVertex3f(fbl.x, fbl.y, fbl.z); glVertex3f(fbl.x, fbl.y, fbl.z); glVertex3f(ftl.x, ftl.y, ftl.z);
		// Connecting both rectangles
		glVertex3f(ntl.x, ntl.y, ntl.z); glVertex3f(ftl.x, ftl.y, ftl.z); glVertex3f(ntr.x, ntr.y, ntr.z); glVertex3f(ftr.x, ftr.y, ftr.z);
		glVertex3f(nbl.x, nbl.y, nbl.z); glVertex3f(fbl.x, fbl.y, fbl.z); glVertex3f(nbr.x, nbr.y, nbr.z); glVertex3f(fbr.x, fbr.y, fbr.z);
		// Lines from eye to plane corners
		glVertex3f(eye.x, eye.y, eye.z); glVertex3f(ntl.x, ntl.y, ntl.z); glVertex3f(eye.x, eye.y, eye.z); glVertex3f(ntr.x, ntr.y, ntr.z);
		glVertex3f(eye.x, eye.y, eye.z); glVertex3f(nbl.x, nbl.y, nbl.z); glVertex3f(eye.x, eye.y, eye.z); glVertex3f(nbr.x, nbr.y, nbr.z);


	glEnd();
	//glEnable(GL_LIGHTING);
	glPopAttrib();
}

void DrawText2D(float x, float y, const char* text, void* font) {
	glRasterPos2f(x, y);
	for (const char* c = text; *c != '\0'; ++c) {
		glutBitmapCharacter(font, *c);
	}
}