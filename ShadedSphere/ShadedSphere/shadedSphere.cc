//
// File: shadedSphere.cc
//
// Author: Douglas Harms
//
// Description: Draws 2 spheres shaded with 2 light sources
//

#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>
using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30;       // eye position to z-axis
static float phi=30;          // eye position to x-z plane
static float dist=5.0;

// display options
bool displayLightSpheres;     // true=>display spheres at light sources

void init(void)
{
  // define light property parameters
  GLfloat lightAmbient[] = {0.0, 0.0, 0.2, 1.0};
  GLfloat lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightSpecular[]= {1.0, 1.0, 1.0, 1.0};
  
  // define material property parameters
  GLfloat matSpecular[]  = {1.0, 1.0, 1.0, 1.0};
  GLfloat matShininess =   100.0;


  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // set up ambient, diffuse, and specular components for the lights
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
  
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
  
  // We'll use glColor to set the diffuse and ambient material properties
  glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable (GL_COLOR_MATERIAL);

  // enable light sources
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHT1);

  // define default material properties
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, matShininess);


  // enable other things
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_CULL_FACE);
  glShadeModel (GL_SMOOTH);
  //glShadeModel(GL_FLAT);

  // initialize options
  displayLightSpheres = true;
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

void display(void)
{
  // define light source positions
  GLfloat light0Pos[] = {0.0, 2.0, 0.0, 1.0};
  GLfloat light1Pos[] = {0.0, 0.0, 0.0, 1.0};
  
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // position light1 to be on the viewer
  glLoadIdentity ();
  glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);

  // position the camera
  setCamera ();

  // place light source 0 in the world
  glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
  
  // do not light the axes or light source spheres
  glDisable (GL_LIGHTING);

  // draw axes
  glBegin(GL_LINES);
  // x in red
  glColor3f(1.0,0.0,0.0);
  glVertex3i(0,0,0);
  glVertex3i(2,0,0);

  // y in green
  glColor3f (0.0, 1.0, 0.0);
  glVertex3i (0,0,0);
  glVertex3i (0,2,0);

  // z in blue
  glColor3f (0.0, 0.0, 1.0);
  glVertex3i (0,0,0);
  glVertex3i (0,0,2);
  glEnd();

  // draw small spheres at the light sources if selected
  if (displayLightSpheres)
    {
      glPushMatrix();
      glColor3f (1.0, 1.0, 1.0);
      glTranslatef (light0Pos[0],light0Pos[1],light0Pos[2]);
      glutSolidSphere (0.05, 10, 10);
      glPopMatrix();
      
      glPushMatrix();
      glColor3f (1.0, 1.0, 1.0);
      glTranslatef (light1Pos[0],light1Pos[1],light1Pos[2]);
      glutSolidSphere (0.05, 10, 10);
      glPopMatrix();
    }

  // enable lighting for the other objects in the world
  glEnable (GL_LIGHTING);

  // draw the two spheres in the world
  glColor3f (1.0, 0.0, 0.0);
  glutSolidSphere(1.0, 50, 50);
  
  glTranslatef (1.5, 0.0, 0.0);
  glColor3f (0.0, 1.0, 0.0);
  glutSolidSphere(0.25, 20, 20);
  
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

  case 27: /* esc */
  case 'Q':
  case 'q':
    exit(0);
    break;

  case 'L':
  case 'l':
    displayLightSpheres = !displayLightSpheres;
    glutPostRedisplay ();
    break;
  }
}

int main(int argc, char** argv)
{
  // initialize glut, creat window, etc.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Very Simple!");
  init();

  // register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // do everything!
  glutMainLoop();
}

