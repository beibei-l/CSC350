//
//  main.cpp
//  Graphics_hw5
//
//  Created by Lu Beibei on 10/29/15.
//  Copyright © 2015 Lu Beibei. All rights reserved.
//

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
#include <time.h>   // time (for random seed)
#include <stdlib.h> // random number stuff (srand, rand)
#include <iostream>

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30;       // eye position to z-axis
static float phi=30;          // eye position to x-z plane
static float dist=5.0;

// define global state variables
int whichLight = 0; // 0 = Viewer   1 = World    2 = Sphere


// define light source positions
GLfloat light0Pos[] = {0.0, 2.0, 0.0, 1.0};
GLfloat light1Pos[] = {2.0, 0.75, 1.0, 1.0};
GLfloat light2Pos[] = {2.0, 0.75, 1.0, 1.0};


// display options
bool displayLightSpheres;
bool animated;
float arc;

// define local functions to draw some basic shapes

// drawUnitPolygon draws a unit regular polygon in the x-y plane centered
// around the origin
void drawUnitPolygon( int numSides )
{
    double theta = 0.0;
    double deltaTheta = 2.0 * M_PI / numSides;
    double x, y;
    
    glBegin( GL_LINE_LOOP );
    for( int i=0; i<numSides; i++ ) {
        x = cos( theta );
        y = sin( theta );
        glVertex3f ( x, y, 0.0 );
        theta += deltaTheta;
    }
    glEnd();
}

// drawUnitCylinder draws a numSided cylinder around the z axis. The cylinder
// extends from z=+1 to z=-1, and the radius of the cylinder is 1.
void drawUnitCylinder( int numSides, int numStacks )
{
    double thetaRight;
    double deltaTheta = 2.0 * M_PI / numSides;
    double xLeft, yLeft, xRight, yRight;
    double zTop, zBottom;
    double deltaZ = 2.0 / numStacks;
    
    // initialize zTop to be the top of the first stack
    zTop = 1.0;
    
    // draw all stacks
    for( int i=0; i<numStacks; i++ ) {
        
        // set zBottom to be the bottom of the current stack.  If we're at the
        // last stack we'll set zBottom to be -1.0 so that we don't accumulate
        // roundoff.
        if( i == numStacks-1 )
            zBottom = -1.0;
        else
            zBottom = zTop - deltaZ;
        
        // initialize left coordinate to (1,0) for the first quad in this stack
        xLeft = 1.0;
        yLeft = 0.0;
        
        // initialize angle of the right side of the first quad
        thetaRight = deltaTheta;
        
        // draw all quads in this stack
        for( int k=0; k<numSides; k++ ) {
            
            // determine the right coordinates for this quad.  If we're at the
            // last quad, set the right coordinate to (1,0) so we don't accumulate
            // roundoff, otherwise the right coordinate is calculated using thetaRight.
            if( k == numSides-1 ) {
                xRight = 1.0;
                yRight = 0.0;
            }
            else {
                xRight = cos( thetaRight );
                yRight = sin( thetaRight );
            }
            
            // draw the outline of the quad
            glBegin( GL_LINE_LOOP );
            glVertex3f( xLeft, yLeft, zTop );
            glVertex3f( xLeft, yLeft, zBottom );
            glVertex3f( xRight, yRight, zBottom );
            glVertex3f( xRight, yRight, zTop );
            glEnd();
            
            // Prepare for the next quad.  The right coordinate of the current quad
            // is the left coordinate of the next quad, and the angle of the right
            // side of the next quad is deltaTheta larger than it was for the
            // current quad.
            xLeft = xRight;
            yLeft = yRight;
            thetaRight += deltaTheta;
        }
        
        // Prepare for next stack.  The bottom of the current stack is the top of the
        // next one
        zTop = zBottom;
    }
}

// drawUnitSphere draws a numSided unit sphere around the origin. The "poles" of
// the sphere are on the z axis
void drawUnitSphere( int numSides, int numStacks )
{
    double thetaRight;
    double deltaTheta = 2.0 * M_PI / numSides;
    double xTopLeft, yTopLeft, xTopRight, yTopRight;
    double xBottomLeft, yBottomLeft, xBottomRight, yBottomRight;
    double zTop, zBottom;
    double rTop, rBottom;
    double deltaPhi = M_PI / numStacks;
    double phiBottom;
    
    // initialize zTop and rTop to be the top of the first stack
    zTop = 1.0;
    rTop = 0.0;
    
    // initialize phiBottom to be the angle to tbe bottom of the first stack
    phiBottom = deltaPhi;
    
    // draw all stacks
    for( int i=0; i<numStacks; i++ ) {
        
        // set zBottom and rBottom to be the bottom of the current stack.  If we're
        // at the last stack we'll set zBottom to be -1.0 and rBottom to 0 so that
        // we don't accumulate roundoff.
        if( i == numStacks-1 ) {
            zBottom = -1.0;
            rBottom = 0.0;
        }
        else {
            zBottom = cos( phiBottom );
            rBottom = sqrt( 1 - zBottom*zBottom );
        }
        
        // initialize left coordinates for the first quad in this stack
        xTopLeft = rTop;
        yTopLeft = 0.0;
        xBottomLeft = rBottom;
        yBottomLeft = 0.0;
        
        // initialize angle of the right side of the first quad
        thetaRight = deltaTheta;
        
        // draw all quads in this stack
        for( int k=0; k<numSides; k++ ) {
            
            // determine the right coordinates for this quad.  If we're at the
            // last quad, set the right coordinate so we don't accumulate
            // roundoff, otherwise the right coordinate is calculated using thetaRight.
            if( k == numSides-1 ) {
                xTopRight = rTop;
                yTopRight = 0.0;
                xBottomRight = rBottom;
                yBottomRight = 0.0;
            }
            else {
                xTopRight = rTop * cos( thetaRight );
                yTopRight = rTop *sin( thetaRight );
                xBottomRight = rBottom * cos( thetaRight );
                yBottomRight = rBottom *sin( thetaRight );
                
            }
            
            // draw the outline of the quad
            glBegin( GL_LINE_LOOP );
            glVertex3f( xTopLeft, yTopLeft, zTop );
            glVertex3f( xBottomLeft, yBottomLeft, zBottom );
            glVertex3f( xBottomRight, yBottomRight, zBottom );
            glVertex3f( xTopRight, yTopRight, zTop );
            glEnd();
            
            // Prepare for the next quad.  The right coordinate of the current quad
            // is the left coordinate of the next quad, and the angle of the right
            // side of the next quad is deltaTheta larger than it was for the
            // current quad.
            xTopLeft = xTopRight;
            yTopLeft = yTopRight;
            xBottomLeft = xBottomRight;
            yBottomLeft = yBottomRight;
            thetaRight += deltaTheta;
        }
        
        // Prepare for next stack.  The bottom of the current stack is the top of the
        // next one
        zTop = zBottom;
        rTop = rBottom;
        phiBottom += deltaPhi;
    }
}

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
    
    // define light property parameters
    GLfloat lightAmbient[] = {0.0, 0.0, 0.2, 1.0};
    GLfloat lightAmbient2[] = {0.3, 0.0, 0.0, 1.0};
    GLfloat lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat lightSpecular[]= {1.0, 1.0, 1.0, 1.0};
    
    // define material property parameters
    GLfloat matSpecular[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat matShininess =   100.0;
    
    // define clear color to be black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    
    // set up ambient, diffuse, and specular components for the lights
    
    // Camera
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    // World
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    
    // Sphere
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular);
    
    // We'll use glColor to set the diffuse and ambient material properties
    glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
    
    // enable light sources
    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHT2);
    
    // define default material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    
    
    
    // shade polygons as flat surfaces
    glShadeModel(GL_FLAT);
    
    // do hidden surface removal
    glEnable (GL_DEPTH_TEST);
    
    
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, 1 );    //enables lighting for both front and back faces of polygons.
    
    
    glEnable( GL_NORMALIZE);
    
    animated = false;
    
    // initialize options
    displayLightSpheres = true;
    arc =0.0;
    
    
    
}

void display(void)
{
    // define material property parameters
    GLfloat matSpecular[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat matShininess =   100.0;
    
    
    
    // define default material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    
    // clear frame buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // Light 0 - Camera
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    // set viewpoint
    glLoadIdentity();
    
    // set the camera
    setCamera();
    
    
    // Light 1 - World
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    
    // do not light the axes or light source spheres
    glDisable (GL_LIGHTING);
    
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
    
    // Define three light sources in the world: one attached to the viewer,
    // one attached to the world,
    // and one attached to the (moving) sphere.
    
    // draw small spheres at the light sources if selected
    if (displayLightSpheres)
    {
        glPushMatrix();
        glColor3f (1.0, 1.0, 1.0);
        glTranslatef (light1Pos[0],light1Pos[1],light1Pos[2]);
        glutSolidSphere (0.05, 10, 10);
        glPopMatrix();
        
    }
    
    glEnable(GL_LIGHTING);
    
    
    //Draw Cylinder
    glPushMatrix();
    glColor3f (1.0, 0.0, 1.0);
    glScalef(0.5, 0.5, 1);
    drawUnitCylinder(100, 200);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(arc, 1, 0, 0);
    glColor3f (0.0, 0.6, 0.5);
    glPushMatrix();
    // Draw Sphere
    glRotatef(arc, 1, 0, 0);
    glTranslatef (0,-1,0);
    glScalef(0.3, 0.3, 0.3);
    drawUnitSphere(500, 500);
    glPopMatrix();
    // Light 2 - Sphere
    glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
    if (displayLightSpheres)
    {
        glPushMatrix();
        glColor3f (1.0, 1.0, 1.0);
        glTranslatef (light2Pos[0],light2Pos[1],light2Pos[2]);
        glutSolidSphere (0.05, 10, 10);
        glPopMatrix();
    }
    glPopMatrix();
    
    
    
    glPushMatrix();
    glColor3f (1.0, 1.0, 0.0);
    glTranslatef (-1,0.5,0);
    drawUnitPolygon(6);
    glPopMatrix();
    
    // enable lighting for the other objects in the world
    glEnable (GL_LIGHTING);
    
    // display things
    glutSwapBuffers();
}

void timer (int val)
{
    
    if( animated )
    {
        // register timer to go off in 1/10 second
        glutTimerFunc( 1.0, timer, 0 );
        arc+=1.0*2;
        glutPostRedisplay();
    }
    
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
            
        case '!': // shift-1, selects light 1 (attached to the viewer) for manipulation
            whichLight = 0;
            glutPostRedisplay();
            break;
        case '@': //shift-2, selects light 2 (attached to the world) for manipulation
            whichLight = 1;
            glutPostRedisplay();
            break;
        case '#': // shift-3, selects light 3 (attached to the sphere) for manipulation
            whichLight = 2;
            glutPostRedisplay();
            break;
            
        case 'O':
        case 'o':   // 'O'. 'o': turn the selected light on/off
            switch( whichLight ) {
                case 0:
                    if (glIsEnabled(GL_LIGHT0)) {
                        glDisable(GL_LIGHT0);
                    } else {
                        glEnable(GL_LIGHT0);
                    }
                    break;
                case 1:
                    if (glIsEnabled(GL_LIGHT1)) {
                        glDisable(GL_LIGHT1);
                    } else {
                        glEnable(GL_LIGHT1);
                    }
                    break;
                case 2:
                    if (glIsEnabled(GL_LIGHT2)) {
                        glDisable(GL_LIGHT2);
                    } else {
                        glEnable(GL_LIGHT2);
                    }
                    break;
            }
            glutPostRedisplay ();
            break;
            
        case 'X':   // 'X' : increases the x coordinate of the currently selected light
            switch( whichLight ) {
                case 0:
                    light0Pos[0] += 0.5;
                    break;
                case 1:
                    light1Pos[0] += 0.5;
                    break;
                case 2:
                    light2Pos[0] += 0.5;
                    break;
            }
            glutPostRedisplay ();
            break;
        case 'x':   // 'x' : decreases the x coordinate of the currently selected light
            switch( whichLight ) {
                case 0:
                    light0Pos[0] -= 0.5;
                    break;
                case 1:
                    light1Pos[0] -= 0.5;
                    break;
                case 2:
                    light2Pos[0] -= 0.5;
                    break;
            }
            glutPostRedisplay ();
            break;
        case 'Y':   // 'Y' : increases the y coordinate of the currently selected light
            switch( whichLight ) {
                case 0:
                    light0Pos[1] += 0.5;
                    break;
                case 1:
                    light1Pos[1] += 0.5;
                    break;
                case 2:
                    light2Pos[1] += 0.5;
                    break;
            }
            glutPostRedisplay ();
            break;
        case 'y':   // 'y' : decreases the y coordinate of the currently selected light
            switch( whichLight ) {
                case 0:
                    light0Pos[1] -= 0.5;
                    break;
                case 1:
                    light1Pos[1] -= 0.5;
                    break;
                case 2:
                    light2Pos[1] -= 0.5;
                    break;
            }
            glutPostRedisplay ();
            break;
        case 'Z':   // 'Z' : increases the z coordinate of the currently selected light
            switch( whichLight ) {
                case 0:
                    light0Pos[2] += 0.5;
                    break;
                case 1:
                    light1Pos[2] += 0.5;
                    break;
                case 2:
                    light2Pos[2] += 0.5;
                    break;
            }
            glutPostRedisplay ();
            break;
        case 'z':   // 'z' : decreases the z coordinate of the currently selected light
            switch( whichLight ) {
                case 0:
                    light0Pos[2] -= 0.5;
                    break;
                case 1:
                    light1Pos[2] -= 0.5;
                    break;
                case 2:
                    light2Pos[2] -= 0.5;
                    break;
            }
            
            glutPostRedisplay ();
            break;
            
            // 'L' or 'l': toggles the drawing of small spheres at the location of all light sources
        case 'L':
        case 'l':
            displayLightSpheres = !displayLightSpheres;
            glutPostRedisplay ();
            break;
            
        case 'A': // toggle animation
        case 'a':
            animated = !animated;
            if(animated)
                glutTimerFunc(100, timer, 0);
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

