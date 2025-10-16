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

// Window dimensions
int winW = 640, winH = 480; // Must start with 640x480

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

	switch (key) {
	case 27: // ESC key
		exit(0);
		break;
	case '`':
	case '~':
		debugPrint = !debugPrint;
		std::cout << "Debug print " << (debugPrint ? "enabled" : "disabled") << std::endl;
		break;
	case 'I':
	case 'i':
		PrintInstructions();
		break;
	case 'C':
	case 'c':
		clearScreen = !clearScreen;
		glutPostRedisplay();
		break;
	default:
		break;
	}
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


void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (clearScreen) {
		glutSwapBuffers();
		return;
	}

	glColor3f(1.0, 0.0, 0.0); // R=1,G=0,B=0 -> red color
	// drawing a square
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
	// end drawing
	glFlush();
	glutSwapBuffers(); // swap buffers 
}

static void Reshape(int w, int h) {
	winW = w;
	winH = h;
	glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(winW, winH); // window size
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");

	PrintInstructions();

	glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D
	glClearColor(0.0, 0.0, 0.0, 1.0); // clear the window screen
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Callback functions
	glutDisplayFunc(MyDisplay);			// Call the drawing function
	glutReshapeFunc(Reshape);			// Call the reshape function
	glutKeyboardFunc(KeyboardInput);	// Call the keyboard function
	glutSpecialFunc(SpecialInput);		// Call the special keyboard function

	glutMainLoop();
	return 0;
}
