//
// File: hw2.cc
//
//  Author: Beibei Lu
//

// MacOS, Windows, Linux universal openGL framework
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30;       // eye position to z-axis
static float phi=30;          // eye position to x-z plane
static float dist=5.0;

// define global state variables
int numSides;
int numStacks;
bool displayCylinder;
bool displayPolygon;
bool displaySphere;

float deltaTheta;
float density;

// define local functions to draw some basic shapes

// drawUnitPolygon draws a unit regular polygon in the x-y plane centered
// around the origin
void drawUnitPolygon( int numSides )
{
    
    deltaTheta = 2.0 * M_PI / numSides;
    glColor3f(1.0,0.0,1.0);
    // Generates vertex points of the polygon.
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSides; i++) {
        glVertex3f(cos(deltaTheta*(float)i),sin(deltaTheta*(float)i), 0.0);
    }
    glEnd();
    
}

// drawUnitCylinder draws a numSided cylinder around the z axis. The cylinder
// extends from z=+1 to z=-1, and the radius of the cylinder is 1.  numStacks
// layers are used.
void drawUnitCylinder( int numSides, int numStacks )
{
    
    deltaTheta = 2.0 * M_PI/numSides;
    density = 2.0/numStacks;
    glColor3f(1.0,1.0,0.0);
    

    for(int i = 0; i < numStacks + 1; i++)
    {
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j< numSides; j++)
        {
            glVertex3f(cos(deltaTheta*(float)j),sin(deltaTheta*(float)j), -1 + i * density);
        }
        glEnd();
    }

    for (int i = 0; i < numStacks; i++)
    {
        
        for(int j = 0; j < numSides; j++)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f(cos(deltaTheta*(float)j),sin(deltaTheta*(float)j), 1);
            glVertex3f(cos(deltaTheta*(float)j),sin(deltaTheta*(float)j), -1);
            glEnd();
        }
    }
}
//The code will be similar to drawUnitCylinder except that
//the radius of each slice is calculated by r = sqrt(1 - z^2)
void drawUnitSphere( int numSides, int numStacks)
{
    
    
    for(int i = 0; i < numStacks + 1; i++)
    {
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j< numSides; j++)
        {
            glVertex3f(cos(deltaTheta*(float)j),sin(deltaTheta*(float)j), -1 + i * density);
        }
        glEnd();
    }
    
    for (int i = 0; i < numStacks; i++)
    {
        
        for(int j = 0; j < numSides; j++)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f(cos(deltaTheta*(float)j),sin(deltaTheta*(float)j), 1);
            glVertex3f(cos(deltaTheta*(float)j),sin(deltaTheta*(float)j), -1);
            glEnd();
        }
    }


    
};

void setCamera(void)
{
    // sets the camera position in the world
    
    // set the view transformation
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -dist);
    glRotatef(phi, 1.0, 0.0, 0.0);
    glRotatef(theta, 0.0, 1.0, 0.0);
}

void init(void)
{
    // define clear color to be black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // shade polygons as flat surfaces
    glShadeModel(GL_FLAT);
    
    // do hidden surface removal
    glEnable (GL_DEPTH_TEST);
    
    // initialize state variables
    numSides = 3;
    numStacks = 10;
    displayCylinder = false;
    displaySphere = false;
    displayPolygon = true;
    
}

void display(void)
{
    // clear frame buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // set viewpoint
    glLoadIdentity();
    
    // set the camera
    setCamera();
    
    // draw things in the world
    
    // let's draw the axes first
    glBegin(GL_LINES);
    // x in red
    glColor3f(1.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(2.0,0.0,0.0);
    
    // y in green
    glColor3f (0.0, 1.0, 0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f (0.0,2.0,0.0);
    
    // z in blue
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f (0.0,0.0,2.0);
    glEnd();
    
    // let's draw a polygon or cylinder
    // TODO: draw a sphere
    glColor3f( 1.0, 0.0, 1.0 );
    if( displayCylinder )
        drawUnitCylinder( numSides, numStacks );
    else if( displayPolygon )
        drawUnitPolygon( numSides );
    else if (displaySphere)
        drawUnitSphere( numSides, numStacks);
    
    // display things
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.01, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 27: /* esc */
        case 'Q':
        case 'q':
            exit(0);
            break;
            
        case '4': // left
            theta += 5;
            glutPostRedisplay();
            break;
        case '6': //right
            theta -= 5;
            glutPostRedisplay();
            break;
        case '2': //down
            phi -= 5;
            // don't let elevation go past 90 or -90
            if (phi>90.0)
                phi = 90.0;
            else if (phi < -90.0)
                phi = -90.0;
            glutPostRedisplay();
            break;
        case '8': //up
            phi += 5;
            // don't let elevation go past 90 or -90
            if (phi>90.0)
                phi = 90.0;
            else if (phi < -90.0)
                phi = -90.0;
            glutPostRedisplay();
            break;
        case '3': //pgDn (back up)
            dist += 0.5;
            glutPostRedisplay();
            break;
        case '9': //pgUp (move forward)
            dist -= 0.5;
            glutPostRedisplay();
            break;
        case 'c': //set displayCylinder to true and displayPolygon, displaySphere to false
        case 'C':
            displayCylinder = true;
            displaySphere = false;
            displayPolygon = false;
            glutPostRedisplay();
            break;
        case 'p': //set displayPolygon to true and displayCylinder, displaySphere to false
        case 'P':
            displayCylinder = false;
            displaySphere = false;
            displayPolygon = true;
            glutPostRedisplay();
            break;
        case 's': //set displaySphere to true and displayCylinder, displayPolygon to false
        case 'S':
            displayCylinder = false;
            displaySphere = true;
            displayPolygon = false;
            glutPostRedisplay();
            break;
        case '<': //decrease the number of sides; can’t be less than 3
            if (numSides > 3)
                numSides--;
            glutPostRedisplay();
            break;
        case '>': //increase the number of sides
            numSides++;
            glutPostRedisplay();
            break;
        case '+': //increase the number of stacks of the cylinder
            numStacks++;
            glutPostRedisplay();
            break;
        case '-': //decrease the number of stacks of the cylinder; can’t be less than 1
            if (numStacks > 1)
                numStacks--;
            glutPostRedisplay();
            break;
        
            
    }
}

int main(int argc, char** argv)
{
    // initialize glut, create window, etc.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Very Simple!");
    
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

