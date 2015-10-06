//
// File: bounceBallViewDemo.cc
// Author: Douglas Harms
// Description: animate two bouncing balls, with the viewer being able
// to change from the "world" view, ball1 view, or ball2 view
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
bool animate = false;

float ball1Theta=0.0;
float ball2XPos=0.0;
float ball2Inc = 0.1;
float ball2Theta = 0.0;

int whichView = 1;   // 1=world, 2=ball1, 3=ball2

// set the camera in the world at spherical coordinate (dist, theta, phi)
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
  glTranslatef( -0.5, 0.0, 0.0 );
  glRotatef( -90.0, 1.0, 0.0, 0.0 );
  glTranslatef( -1.5, 0.0, 0.0 );
  glRotatef( -ball1Theta, 0.0, 0.0, 1.0 );
}

void ball2View()
{
  glLoadIdentity();
  glRotatef( -90.0, 1.0, 0.0, 0.0 );
  glTranslatef( -0.5, 0.0, 0.0 );
  glRotatef( -ball2Theta, 0.0, 1.0, 0.0 );
  glTranslatef( -ball2XPos, 0.0, 0.0 );
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

// draw a radius 1 wire sphere centered around the origin.  The longitude
// lines (i.e., north and south poles) will meet on the y-axis.
void drawSphere ()
{
  glPushMatrix ();
  glRotatef (90.0, 1.0, 0.0, 0.0);
  glutWireSphere (1.0, 20, 20);
  glPopMatrix ();
  glBegin( GL_LINES );
  glVertex3f( 0.0, 1.5, 0.0 );
  glVertex3f( 0.0, -1.5, 0.0 );
  glEnd();
}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

  // draw the axes in the "world"
  drawAxes();

  // draw ball1
  glPushMatrix();
  glRotatef( ball1Theta, 0.0, 0.0, 1.0 );
  glTranslatef( 1.5, 0.0, 0.0 );
  glPushMatrix();
  glScalef( 0.5, 0.5, 0.5 );
  glColor3f( 1.0, 0.0, 0.0 );
  drawSphere();
  glPopMatrix();

  // go to surface of ball1 and draw small axes
  glRotatef( 90.0, 1.0, 0.0, 0.0 );  
  glTranslatef( 0.5, 0.0, 0.0 );
  glScalef( 0.3, 0.3, 0.3 );
  drawAxes();

  // return to world frame
  glPopMatrix();

  // draw ball2
  glTranslatef( ball2XPos, 0.0, 0.0 );
  glRotatef( ball2Theta, 0.0, 1.0, 0.0 );
  glColor3f( 0.0, 0.0, 1.0 );
  glPushMatrix();
  glScalef( 0.5, 0.5, 0.5 );
  drawSphere();
  glPopMatrix();

  // go to surface and draw small axes
  glTranslatef( 0.5, 0.0, 0.0 );
  glRotatef( 90.0, 1.0, 0.0, 0.0 );
  glScalef( 0.3, 0.3, 0.3 );
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

void timer( int val )
{
  if( !animate )
    return;
  glutTimerFunc( 100, timer, 0 );
  ball1Theta += 5;
  ball2Theta += 3;
  ball2XPos += ball2Inc;
  if( ball2XPos > 2.0 )
    {
      ball2Inc = -ball2Inc;
      ball2XPos = 2.0;
    }
  if( ball2XPos < -2.0 )
    {
      ball2Inc = -ball2Inc;
      ball2XPos = -2.0;
    }
  glutPostRedisplay();

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
    
  case 'A':
  case 'a':
    animate = !animate;
    if( animate )
      glutTimerFunc( 100, timer, 0 );
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

