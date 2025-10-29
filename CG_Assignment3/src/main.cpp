// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
/* CONTROLS: 
	 Keyboard:
	   'w': Display the wireframe model (edge lines) of entire scene
	   's': Display the solid model of entire scene
	   'c': Toggle collider display on/off
	   'a': Toggle axis display on/off at world origin
	   'b': Toggle bullet speed (slow, fast, very fast)
	   'm': Toggle robot motion
	   F1: Toggle fullscreen/windowed mode
	   F2: Toggle First Person View(FPV)/Entire Scene View(ESV)
	   Up: Move the camera forward
	   Down: Move the camera backward
	   Left: Rotate the camera left
	   Right: Rotate the camera right
	   Space: Shoot a bullet
	   ESC: Exit the program
	 Mouse:
	   Right click: Open the context menu

*/
// ====================================================================
// ====================================================================
#include <GL/glut.h>
#include <GL/freeglut_std.h>
#include <iostream>
#include "robots.hpp"
#include "game.hpp"

// Window dimensions
int winW = 640, winH = 480; // Must start with 640x480
Game game;

bool clearScreen = false; // Toggle to clear the screen
bool debugPrint = true; // Toggle to print debug information

// ====================================================================
// Forward declarations

static void PrintInstructions();
static void KeyboardInput(unsigned char key, int x, int y);
static void SpecialInput(int key, int x, int y);
static void MyDisplay();
static void Reshape(int w, int h);


// ====================================================================

// Testing robot object (remove before use)
Robot g_robot;
Robot g_robot_2(Vector3(0.0f, 10.0f, -30.0f), 10.0f);

float camX = 0.0f;
float camY = 20.0f;
float camZ = 40.0f;
float camYaw = 0.0f; // radians

RenderMode g_rm = RenderMode::Solid;

// Temp ground
//void drawGround() {
//	float s = 200.0f;
//
//	// solid ground quad
//	glColor3f(0.2f, 0.2f, 0.2f);
//	glBegin(GL_QUADS);
//	glVertex3f(-s, 0.0f, -s);
//	glVertex3f(s, 0.0f, -s);
//	glVertex3f(s, 0.0f, s);
//	glVertex3f(-s, 0.0f, s);
//	glEnd();
//
//	// grid lines to help visualize scale
//	glColor3f(0.3f, 0.3f, 0.3f);
//	glLineWidth(1.0f);
//	glBegin(GL_LINES);
//	for (float i = -s; i <= s; i += 10.0f) {
//		// lines parallel to Z
//		glVertex3f(i, 0.01f, -s);
//		glVertex3f(i, 0.01f, s);
//		// lines parallel to X
//		glVertex3f(-s, 0.01f, i);
//		glVertex3f(s, 0.01f, i);
//	}
//	glEnd();
//}


static void PrintInstructions() {
	std::cout << "Instructions:" << std::endl
		<< "Keyboard------------------------------------------" << std::endl
		<< "w: Display wireframe model" << std::endl
		<< "s: Display solid model" << std::endl
		<< "c: Toggle collider display on/off" << std::endl
		<< "a: Toggle axis display on/off at world origin" << std::endl
		<< "b: Toggle bullet speed (slow, fast, very fast)" << std::endl
		<< "m: Toggle robot motion" << std::endl
		<< "F1: Toggle fullscreen/windowed mode" << std::endl
		<< "F2: Toggle First Person View(FPV)/Entire Scene View(ESV)" << std::endl
		<< "Up: Move the camera forward" << std::endl
		<< "Down: Move the camera backward" << std::endl
		<< "Left: Rotate the camera left" << std::endl
		<< "Right: Rotate the camera right" << std::endl
		<< "Space: Shoot a bullet" << std::endl
		<< "ESC: Exit the program" << std::endl
		<< "Mouse---------------------------------------------" << std::endl
		<< "Right click: Open the context menu" << std::endl 
		<< std::endl 
		<< "Press 'i' at any time to bring up this information again" << std::endl << std::endl;
}

static void KeyboardInput(unsigned char key, int x, int y) {
	if (debugPrint) {
		std::cout << "[Debug][KeyboardInput] Key pressed: " << key << " at (" << x << ", " << y << ")" << std::endl;
	}
	game.handleKey(key);

}

static void SpecialInput(int key, int x, int y) {
	if (debugPrint) {
		std::cout << "[Debug][SpecialInput] Special key pressed: " << key << " at (" << x << ", " << y << ")" << std::endl;
	}
	switch (key) {
	case GLUT_KEY_F1:
		// Toggle fullscreen
		break;
	case GLUT_KEY_F2:
		// Toggle view mode
		break;
	case GLUT_KEY_UP:
		// Move camera forward
		break;
	case GLUT_KEY_DOWN:
		// Move camera backward
		break;
	case GLUT_KEY_LEFT:
		// Rotate camera left
		break;
	case GLUT_KEY_RIGHT:
		// Rotate camera right
		break;
	default:
		break;
	}
}

// Temp camera
void setCamera(int width, int height) {
	// projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60.0,                      // fov
		(float)width / (float)height,// aspect
		1.0,                       // near
		1000.0                     // far
	);

	// view (simple yaw around Y axis)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float lx = sinf(camYaw);
	float lz = -cosf(camYaw);

	gluLookAt(
		camX, camY, camZ,          // camera pos
		camX + lx, camY, camZ + lz,// look target
		0.0f, 1.0f, 0.0f           // up
	);
}

void MyDisplay() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//if (clearScreen) {
	//	glutSwapBuffers();
	//	return;
	//}

	//setCamera(winW, winH);

	//glLineWidth(2.0f);
	//glBegin(GL_LINES);
	//// X red
	//glColor3f(1, 0, 0);
	//glVertex3f(0, 0, 0);
	//glVertex3f(20, 0, 0);
	//// Y green
	//glColor3f(0, 1, 0);
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 20, 0);
	//// Z blue
	//glColor3f(0, 0, 1);
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 0, 20);
	//glEnd();

	//drawGround();
	//g_robot.draw(g_rm);
	//g_robot_2.draw(g_rm);

	//glFlush();
	//glutSwapBuffers(); // swap buffers 

	game.draw(winW, winH);
}

static void Reshape(int w, int h) {
	winW = w;
	winH = h;
	glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
	glutPostRedisplay();
}

void Timer(int value) {
	game.update(0.016f); // Approx. 60 FPS
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(winW, winH); // window size
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing"); // Rename window title

	PrintInstructions();

	glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D
	glClearColor(0.0, 0.0, 0.0, 1.0); // clear the window screen
	//glMatrixMode(GL_PROJECTION);

	game.init();

	// Callback functions
	glutDisplayFunc(MyDisplay);			// Call the drawing function
	glutReshapeFunc(Reshape);			// Call the reshape function
	glutKeyboardFunc(KeyboardInput);	// Call the keyboard function
	glutSpecialFunc(SpecialInput);		// Call the special keyboard function

	glutTimerFunc(16, Timer, 0);

	glutMainLoop();
	return 0;
}
