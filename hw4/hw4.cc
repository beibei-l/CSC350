//
//  File: hw4.cc
//
//  Created by Lu Beibei on 10/13/15.
//  Copyright © 2015 Lu Beibei. All rights reserved.
//
// Description: This is an openGL program that has 2 spheres bouncing within
// a cube centered around the origin.  Students will modify this to allow
// the user to place the camera on the spheres.
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

// define global state variables

// theta, phi, and dist are the eye position in 3d polar coordinates
float theta=0;       // eye position to z-axis
float phi=15;          // eye position to x-z plane
float dist=15.0;

bool animate;
float speed;

// data for the spheres' positions
float s1XPos, s1YPos, s1ZPos, s1XTheta, s1YTheta, s1ZTheta, s1Radius;
float s1XDelta, s1YDelta, s1ZDelta;
float s1XThetaDelta, s1YThetaDelta, s1ZThetaDelta;

float s2XPos, s2YPos, s2ZPos, s2XTheta, s2YTheta, s2ZTheta, s2Radius;
float s2XDelta, s2YDelta, s2ZDelta;
float s2XThetaDelta, s2YThetaDelta, s2ZThetaDelta;

int whichView = 1;   // 1=world, 2=ball1, 3=ball2

void setCamera(void)
{
    // set the view transformation
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -dist);
    glRotatef(phi, 1.0, 0.0, 0.0);
    glRotatef(theta, 0.0, 1.0, 0.0);
}

void ball1View()
{
    glLoadIdentity();
    glRotatef( 90.0, 0.0, 0.0, 1.0 );
    glTranslatef( -s1Radius, 0.0, 0.0 );
    glRotatef( -s1ZTheta, 0, 0, 1 );
    glRotatef( -s1YTheta, 0, 1, 0 );
    glRotatef( -s1XTheta, 1, 0, 0 );
    glTranslatef( -s1XPos, -s1YPos, -s1ZPos );
}
void ball2View()
{
    
    glLoadIdentity();
    
    glRotatef( 90.0, 0.0, 0.0, 1.0 );
    glTranslatef( -s2Radius, 0.0, 0.0 );
    glRotatef( -s2ZTheta, 0, 0, 1 );
    glRotatef( -s2YTheta, 0, 1, 0 );
    glRotatef( -s2XTheta, 1, 0, 0 );
    glTranslatef( -s2XPos, -s2YPos, -s2ZPos );
    
}

// return a random integer between low  and high
double randomf(double low, double high)
{
    double r =  ((double)rand())/RAND_MAX;
    return r*(high-low) + low;
}

void init(void)
{
    // define clear color to be black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // shade polygons as flat surfaces
    glShadeModel(GL_FLAT);
    
    // do depth testing
    glEnable( GL_DEPTH_TEST );
    
    // initialize data structures
    // seed the random number generator
    srand (time (0));
    
    // no animation yet
    animate = false;
    
    // normal speed
    speed = 1.0;
    
    // initialize world data structures
    s1Radius = 0.75;
    // position and movemnet are all random
    s1XPos = randomf( -5.0+s1Radius, 5.0-s1Radius );
    s1YPos = randomf( -5.0+s1Radius, 5.0-s1Radius );
    s1ZPos = randomf( -5.0+s1Radius, 5.0-s1Radius );
    s1XTheta = randomf( -180.0, 180.0 );
    s1YTheta = randomf( -180.0, 180.0 );
    s1ZTheta = randomf( -180.0, 180.0 );
    s1XDelta = randomf( 0.0, 0.2 );
    s1YDelta = randomf( 0.0, 0.2 );
    s1ZDelta = randomf( 0.0, 0.2 );
    s1XThetaDelta = randomf( -5.0, 5.0 );
    s1YThetaDelta = randomf( -5.0, 5.0 );
    s1ZThetaDelta = randomf( -5.0, 5.0 );
    
    s2Radius = 1.5;
    // position and movement are all random
    s2XPos = randomf( -5.0+s2Radius, 5.0-s2Radius );
    s2YPos = randomf( -5.0+s2Radius, 5.0-s2Radius );
    s2ZPos = randomf( -5.0+s2Radius, 5.0-s2Radius );
    s2XTheta = randomf( -180.0, 180.0 );
    s2YTheta = randomf( -180.0, 180.0 );
    s2ZTheta = randomf( -180.0, 180.0 );
    s2XDelta = randomf( 0.0, 0.2 );
    s2YDelta = randomf( 0.0, 0.2 );
    s2ZDelta = randomf( 0.0, 0.2 );
    s2XThetaDelta = randomf( -5.0, 5.0 );
    s2YThetaDelta = randomf( -5.0, 5.0 );
    s2ZThetaDelta = randomf( -5.0, 5.0 );
    
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.01, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer( int val )
{
    if( animate )
        glutTimerFunc( 100, timer, 0 );
    
    // move the first sphere, bouncing off the edge if it hits it
    s1XPos += s1XDelta * speed;;
    if( s1XPos+s1Radius > 5.0 || s1XPos-s1Radius < -5.0 )
    {
        s1XPos -= s1XDelta;
        s1XDelta = -s1XDelta;
    }
    s1YPos += s1YDelta * speed;;
    if( s1YPos+s1Radius > 5.0 || s1YPos-s1Radius < -5.0 )
    {
        s1YPos -= s1YDelta;
        s1YDelta = -s1YDelta;
    }
    s1ZPos += s1ZDelta * speed;
    if( s1ZPos+s1Radius > 5.0 || s1ZPos-s1Radius < -5.0 )
    {
        s1ZPos -= s1ZDelta;
        s1ZDelta = -s1ZDelta;
    }
    
    // rotate sphere1
    s1XTheta += s1XThetaDelta * speed;
    s1YTheta += s1YThetaDelta * speed;
    s1ZTheta += s1ZThetaDelta * speed;
    
    // move the second sphere, bouncing off the edge if it hits it
    s2XPos += s2XDelta * speed;;
    if( s2XPos+s1Radius > 5.0 || s2XPos-s1Radius < -5.0 )
    {
        s2XPos -= s2XDelta;
        s2XDelta = -s2XDelta;
    }
    s2YPos += s2YDelta * speed;;
    if( s2YPos+s1Radius > 5.0 || s2YPos-s1Radius < -5.0 )
    {
        s2YPos -= s2YDelta;
        s2YDelta = -s2YDelta;
    }
    s2ZPos += s2ZDelta * speed;
    if( s2ZPos+s1Radius > 5.0 || s2ZPos-s1Radius < -5.0 )
    {
        s2ZPos -= s2ZDelta;
        s2ZDelta = -s2ZDelta;
    }
    
    // rotate sphere2
    s2XTheta += s2XThetaDelta * speed;
    s2YTheta += s2YThetaDelta * speed;
    s2ZTheta += s2ZThetaDelta * speed;
    
    // if the spheres intersect, have them bounce off each other
    float dist = sqrt( (s1XPos-s2XPos)*(s1XPos-s2XPos) +
                      (s1YPos-s2YPos)*(s1YPos-s2YPos) +
                      (s1ZPos-s2ZPos)*(s1ZPos-s2ZPos) );
    if( dist <= (s1Radius + s2Radius) )
    {
        // they intersect, so exchange velocities
        float temp;
        temp = s1XDelta;
        s1XDelta = s2XDelta;
        s2XDelta = temp;
        
        temp = s1YDelta;
        s1YDelta = s2YDelta;
        s2YDelta = temp;
        
        temp = s1ZDelta;
        s1ZDelta = s2ZDelta;
        s2ZDelta = temp;
    }
    
    // the world needs redrawn
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
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
            // can't go < -90
            if( phi < -90 )
                phi = -90;
            glutPostRedisplay();
            break;
        case '8': //up
            phi += 5;
            // can't go > 90
            if( phi > 90 )
                phi = 90;
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
            
        case 27: /* esc */
        case 'Q':
        case 'q':
            exit(0);
            break;
            
        case 'A': // toggle animation
        case 'a':
            animate = !animate;
            // register callback if we just turned animation on
            if (animate)
                glutTimerFunc (20,timer,0);
            break;
            
        case '+':
            speed += 0.1;
            break;
        case '-':
            speed -= 0.1;
            if( speed < 0.1 )
                speed = 0.1;
            break;
            
        case '!': // shift-1
            whichView = 1;
            glutPostRedisplay();
            break;
        case '@': //shift-2
            whichView = 2;
            glutPostRedisplay();
            break;
        case '#': // shift-3
            whichView = 3;
            glutPostRedisplay();
            break;
    }
}

/*
 * draw axes in the current frame
 */
void drawAxes()
{
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
}

// draw a radius 1 wire sphere centered around the origin.  The longitude
// lines (i.e., north and south poles) will meet on the y-axis.  A line
// through the north and south poles will also be drawn, as well as a small
// "flag" on the north pole line.
void drawSphere ()
{
    glPushMatrix ();
    glRotatef (90.0, 1.0, 0.0, 0.0);
    glutWireSphere (1.0, 20, 20);
    glPopMatrix ();
    glBegin( GL_LINES );
    glVertex3f( 0.0, 1.5, 0.0 );
    glVertex3f( 0.0, -1.5, 0.0 );
    glVertex3f( 0.0, 1.5, 0.0 );
    glVertex3f( 0.25, 1.5, 0.0 );
    glEnd();
}

void display(void)
{
    // clear frame buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // define view transformation
    switch( whichView )
    {
        case 1:
            setCamera();
            break;
        case 2:
            ball1View();
            break;
        case 3:
            ball2View();
            break;
    }
    
    // draw sphere1
    glPushMatrix(); // push world frame so we can get back here
    glTranslatef( s1XPos, s1YPos, s1ZPos );
    glRotatef( s1XTheta, 1, 0, 0 );
    glRotatef( s1YTheta, 0, 1, 0 );
    glRotatef( s1ZTheta, 0, 0, 1 );
    glColor3f( 1.0, 0.0, 0.0 );
    glPushMatrix();
    glScalef( s1Radius, s1Radius, s1Radius );
    drawSphere();
    glPopMatrix();
    // draw small axes on equator
    glPushMatrix();
    glTranslatef( s1Radius, 0.0, 0.0 );
    glRotatef( -90.0, 0.0, 0.0, 1.0 );
    glScalef( 0.2, 0.2, 0.2 );
    drawAxes();
    glPopMatrix();
    
    // back to world frame
    glPopMatrix();
    
    // draw sphere2
    glPushMatrix(); // push world frame so we can get back here
    glTranslatef( s2XPos, s2YPos, s2ZPos );
    glRotatef( s2XTheta, 1, 0, 0 );
    glRotatef( s2YTheta, 0, 1, 0 );
    glRotatef( s2ZTheta, 0, 0, 1 );
    glColor3f( 1.0, 1.0, 0.0 );
    glPushMatrix();
    glScalef( s2Radius, s2Radius, s2Radius );
    drawSphere();
    glPopMatrix();
    // draw small axes on equator
    glPushMatrix();
    glTranslatef( s2Radius, 0.0, 0.0 );
    glRotatef( -90.0, 0.0, 0.0, 1.0 );
    glScalef( 0.2, 0.2, 0.2 );
    drawAxes();
    glPopMatrix();
    
    // back to world frame
    glPopMatrix();
    
    // draw box
    // "left"
    glBegin( GL_LINE_LOOP );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -5.0, -5.0, 5.0 );
    glVertex3f( -5.0, -5.0, -5.0 );
    glVertex3f( -5.0, 5.0, -5.0 );
    glVertex3f( -5.0, 5.0, 5.0 );
    glEnd();
    
    // "right"
    glBegin( GL_LINE_LOOP );
    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f( 5.0, -5.0, 5.0 );
    glVertex3f( 5.0, -5.0, -5.0 );
    glVertex3f( 5.0, 5.0, -5.0 );
    glVertex3f( 5.0, 5.0, 5.0 );
    glEnd();
    
    // "front"
    glBegin( GL_LINE_LOOP );
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -5.0, 5.0, 5.0 );
    glVertex3f( 5.0, 5.0, 5.0 );
    glVertex3f( 5.0, -5.0, 5.0 );
    glVertex3f( -5.0, -5.0, 5.0 );
    glEnd();
    
    // "back"
    glBegin( GL_LINE_LOOP );
    glColor3f( 0.0, 1.0, 1.0 );
    glVertex3f( -5.0, 5.0, -5.0 );
    glVertex3f( 5.0, 5.0, -5.0 );
    glVertex3f( 5.0, -5.0, -5.0 );
    glVertex3f( -5.0, -5.0, -5.0 );
    glEnd();
    
    glutSwapBuffers();
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
