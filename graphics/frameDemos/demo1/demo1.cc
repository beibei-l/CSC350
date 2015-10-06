//
// File: demo1.cc
// Author: Douglas Harms
// Description:  This program simply draws a square in the xy plane.  Pretty
// boring, actually.  It will be the basis, though, for other more interesting
// programs.
//
//
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>

using namespace std;

// define global state variables

// theta, phi, and dist are the eye position in 3d polar coordinates
float theta=-30;       // eye position to z-axis
float phi=30;        // eye position to x-z plane
float dist=5.0;

void drawSquare()
{
  // draw a unit square in the xy plane centered at the origin
  glBegin(GL_LINE_LOOP);
  glVertex2f (-1.0, -1.0);
  glVertex2f (-1.0, 1.0);
  glVertex2f (1.0, 1.0);
  glVertex2f (1.0, -1.0);
  glEnd ();
}

void setCamera(void)
{
  // don't let elevation go past 90 or -90
  if (phi>90.0)
    phi = 90.0;
  else if (phi < -90.0)
    phi = -90.0;
  
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

}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

  // draw a unit square in white
  glColor3f (1.0, 1.0, 1.0);
  drawSquare ();

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
    glutPostRedisplay();
    break;
  case '8': //up
    phi += 5;
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

