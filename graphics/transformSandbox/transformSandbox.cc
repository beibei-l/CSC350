//
// File: transformSandbox.cc
// Author: Douglas Harms
// Description: This program will help demonstrate the effect of
// transformations.  Students can define any transformation desired and
// see what the effect is.
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

// set the camera in the world at spherical coordinate (dist, theta, phi)
void setCamera(void)
{
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

  // do depth testing
  glEnable (GL_DEPTH_TEST);

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

/*
 * Define a transform
*/
void transform()
{
  // for now, do a scale and translate
  glTranslatef( 0.5, 0.0, -0.7 );
  glScalef( 0.5, 0.5, 0.5 );
}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setCamera();

  // draw the axes in the "world"
  drawAxes();

  // now apply the user-defined transformation
  transform();

  // finally, draw the axes in the new frame
  drawAxes();

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
    
  // keys to change the camera position
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
    if( phi < -90.0 )
      phi = -90;
    glutPostRedisplay();
    break;
  case '8': //up
    phi += 5;
    if( phi > 90.0 )
      phi = 90.0;
    glutPostRedisplay();
    break;
  case '3': //move away
    dist += 0.5;
    glutPostRedisplay();
    break;
  case '9': //move closer
    dist -= 0.5;
    glutPostRedisplay();
    break;
    
  case 27: /* esc */
  case 'Q':
  case 'q':
    exit(0);
    break;
    

    // process other keys here

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

