//
// File: hw3.cc
//
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float eyeTheta = -30;       // eye position to z-axis
static float eyePhi = 30;          // eye position to x-z plane
static float eyeDist = 5.0;

// define global state variables
const int NUM_ITEMS=10;		// max number of different mobile objects
float theta[NUM_ITEMS];		// angles of the mobile objects
float deltaTheta[NUM_ITEMS];// how much each angle changes during each tick
bool animate;

// constants for angle offsets into theta and deltaTheta arrays for each
// type of mobile object
const int CIRCLE_THETA = 0;
const int SQUARE_THETA = 1;
const int TYPE1_THETA = 2;

// define local functions to draw basic mobile objects

// drawUnitPolygon draws a unit regular polygon in the x-y plane centered
// around the origin
void drawUnitPolygon(int numSides) {
	double theta = 0.0;
	double deltaTheta = 2.0 * M_PI / numSides;
	double x, y;

	glBegin( GL_POLYGON );
	for (int i = 0; i < numSides; i++) {
		x = cos(theta);
		y = sin(theta);
		glVertex3f(x, y, 0.0);
		theta += deltaTheta;
	}
	glEnd();
}

//
// Draw a circle on a string
//
// A circle at the end of a string will be drawn.  The top of the string
// will be at the origin of the frame defined when the function is invoked,
// and bottom of the string will be on the negative y axis. A unit circle
// will be drawn at the bottom of the string, rotated around the y-axis by
// CIRCLE_THETA degrees.
//
// Upon return, the current frame will be the same frame as when the function
// was invoked.
//
void circleObject() {
	// save the current frame so we can return later
	glPushMatrix();

	// draw the string
	glBegin( GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, -1, 0);
	glEnd();

	// rotate around the y-axis and move to the end of the string
	glTranslatef(0, -1, 0);
	glRotatef(theta[CIRCLE_THETA], 0, 1, 0);

	// move the frame so the origin is at the center of a unit polygon
	glTranslatef( 0, -1, 0 );

	// We want a vertex to be at the end of the string, and drawUnitPolygon
	// defines the first vertex of the polygon on the x-axis.  Rotate the
	// frame so this vertex will be on the y-axis.
	glRotatef( 90, 0, 0, 1 );

	// draw the circle (approximated by a 30-sided polygon)
	drawUnitPolygon(30);

	// return to original frame
	glPopMatrix();
}

//
// Draw a square on a string
//
// A square at the end of a string will be drawn.  The top of the string
// will be at the origin of the frame defined when the function is invoked,
// and bottom of the string will be on the negative y axis. A unit square
// will be drawn at the bottom of the string, rotated around the y-axis by
// SQUARE_THETA degrees.
//
// Upon return, the current frame will be the same frame as when the function
// was invoked.
//
void squareObject() {
	// save the current frame so we can return later
	glPushMatrix();

	// draw the string
	glBegin( GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, -0.8, 0);
	glEnd();

	// rotate around the y-axis and move to the end of the string
	glTranslatef(0, -0.8, 0);
	glRotatef(theta[SQUARE_THETA], 0, 1, 0);

	// move the frame so the origin is at the center of a unit polygon
	glTranslatef( 0, -1, 0 );

	// We want a vertex to be at the end of the string, and drawUnitPolygon
	// defines the first vertex of the polygon on the x-axis.  Rotate the
	// frame so this vertex will be on the y-axis.
	glRotatef( 90, 0, 0, 1 );

	// draw the square
	drawUnitPolygon(4);

	// return to original frame
	glPopMatrix();
}

//
// Draw a type1 mobile arm on a string
//
// A type 1 arm at the end of a string will be drawn.  The top of the string
// will be at the origin of the frame defined when the function is invoked,
// and bottom of the string will be on the negative y axis. An arm with
// a circle object on one end and a square object at the other end will be
// drawn at the bottom of the string, rotated around the y-axis by
// TYPE1_THETA degrees.
//
// Upon return, the current frame will be the same frame as when the function
// was invoked.
//
void type1Object()
{
	// save the frame so we can return later
	glPushMatrix();

	// draw the string and move to the end of the string
	glColor3f( 1, 1, 1 );
	glBegin( GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, -1.1, 0);
	glEnd();
	glTranslatef(0, -1.1, 0);
	glRotatef(theta[TYPE1_THETA], 0, 1, 0);

	// draw the arm, centered at the end of the string
	glColor3f( 1, 1, 1 );
	glBegin( GL_LINES );
	glVertex3f( 1.5, 0, 0 );
	glVertex3f( -1.5, 0, 0 );
	glEnd();

	// draw a circle dangling from one end of the arm.  After drawing the
	// circle, the frame will be back to the center of the arm.
	glPushMatrix();
	glTranslatef( -1.5, 0., 0 );
	glColor3f( 1, 0, 0 );
	circleObject();
	glPopMatrix();

	// draw a square at the other end of the arm
	glTranslatef( 1.5, 0, 0 );
	glColor3f( 0, 1, 1 );
	glScalef( 0.75, 0.75, 0.75 );
	squareObject();

	// return to original frame
	glPopMatrix();
}


void setCamera(void) {
	// sets the camera position in the world

	// set the view transformation
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -eyeDist);
	glRotatef(eyePhi, 1.0, 0.0, 0.0);
	glRotatef(eyeTheta, 0.0, 1.0, 0.0);
}

void init(void) {
	// define clear color to be black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// shade polygons as flat surfaces
	glShadeModel(GL_FLAT);

	// do hidden surface removal
	glEnable(GL_DEPTH_TEST);

	// initialize state variables
	for( int i=0; i<NUM_ITEMS; i++ )
	{
		theta[i] = 0.0;
	}

	deltaTheta[CIRCLE_THETA] = 2.0;
	deltaTheta[SQUARE_THETA] = -5.0;
	deltaTheta[TYPE1_THETA] = 3.0;

	animate = false;
}

void display(void) {
	// clear frame buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set viewpoint
	glLoadIdentity();

	// set the camera
	setCamera();

	// draw things in the world

	// the top of the main string will be at (0, 2, 0 );
	glTranslatef( 0, 2, 0 );

	// the main mobile arm will be a type 2 arm
	type1Object();

	// display things
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 0.01, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

void timer( int val )
{
	if( animate )
	{
		// register timer to go off in 1/10 second
		glutTimerFunc( 100, timer, 0 );

		// update all angles
		for( int i=0; i<NUM_ITEMS; i++ )
		{
			theta[i] += deltaTheta[i];
		}

		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: /* esc */
	case 'Q':
	case 'q':
		exit(0);
		break;

	case '4': // left
		eyeTheta += 5;
		glutPostRedisplay();
		break;
	case '6': //right
		eyeTheta -= 5;
		glutPostRedisplay();
		break;
	case '2': //down
		eyePhi -= 5;
		// don't let elevation go past 90 or -90
		if (eyePhi > 90.0)
			eyePhi = 90.0;
		else if (eyePhi < -90.0)
			eyePhi = -90.0;
		glutPostRedisplay();
		break;
	case '8': //up
		eyePhi += 5;
		// don't let elevation go past 90 or -90
		if (eyePhi > 90.0)
			eyePhi = 90.0;
		else if (eyePhi < -90.0)
			eyePhi = -90.0;
		glutPostRedisplay();
		break;
	case '3': //pgDn (back up)
		eyeDist += 0.5;
		glutPostRedisplay();
		break;
	case '9': //pgUp (move forward)
		eyeDist -= 0.5;
		glutPostRedisplay();
		break;

	case 'a':
	case 'A':
		animate = !animate;
		if( animate )
			glutTimerFunc( 100, timer, 0 );
		break;
	}
}

int main(int argc, char** argv) {
	// initialize glut, create window, etc.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Mobile App");

	// initialize program data structures
	init();

	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	// do everything!
	glutMainLoop();

	return 0;

}

