//
// File: shadedSphere2.cc
//
// Author: Douglas Harms
//
// Description: Draws 2 spheres and a polygon shaded with 3 light sources.
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

void drawTriangle (float x1, float y1, float z1,
		   float x2, float y2, float z2,
		   float x3, float y3, float z3)
{
  float u[3], v[3], n[3];

  // calculate u=P2-P1
  u[0] = x2-x1;
  u[1] = y2-y1;
  u[2] = z2-z1;

  // calculate v=P3-P1
  v[0] = x3-x1;
  v[1] = y3-y1;
  v[2] = z3-z1;

  // calculate n=u x v
  n[0] = u[1]*v[2]-u[2]*v[1];
  n[1] = u[2]*v[0]-v[0]*u[2];
  n[2] = u[0]*v[1]-u[1]*v[0];

  // define the triangle
  glNormal3fv (n);
  glBegin (GL_POLYGON);
  glVertex3f (x1, y1, z1);
  glVertex3f (x2, y2, z2);
  glVertex3f (x3, y3, z3);
  glEnd ();
}

void init(void)
{
  // define light property parameters
  GLfloat lightAmbient[] = {0.0, 0.0, 0.2, 1.0};
  GLfloat lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightSpecular[]= {1.0, 1.0, 1.0, 1.0};
  
  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // set up ambient, diffuse, and specular components for the lights
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
  
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
  
  // We'll use glColor to set the diffuse and ambient material properties
  glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable (GL_COLOR_MATERIAL);

  // enable light sources
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHT1);

  // enable other things
  glEnable (GL_DEPTH_TEST);
  //glEnable (GL_CULL_FACE);           // don't render back faces of polygons
  glEnable (GL_NORMALIZE);           // make all normals unit vectors
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
  // define material property parameters
  GLfloat matSpecular[]  = {1.0, 1.0, 1.0, 1.0};
  GLfloat matShininess =   100.0;

  // define light source positions
  GLfloat light0Pos[] = {0.0, 2.0, 0.0, 1.0};
  GLfloat light1Pos[] = {2.0, 0.75, 1.0, 1.0};
  
  // define default material properties
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // position the camera in the world
  glLoadIdentity ();
  setCamera ();

  // place the light source
  glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
  glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
  
  // do not light the axes or light source spheres
  glEnable (GL_LIGHTING);

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
  glPushMatrix();
  glColor3f (1.0, 0.0, 0.0);
  glutSolidSphere(1.0, 50, 50);
  
  glTranslatef (1.5, 0.0, 0.0);
  glColor3f (0.0, 1.0, 0.0);
  glutSolidSphere(0.25, 20, 20);
  glPopMatrix();

  // draw a polygon in the x-y plane
  glNormal3f (0.0, 0.0, 1.0);
  glColor3f (0.7, 0.1, 0.9);
  glBegin (GL_POLYGON);
  glVertex3f (-1.0, 1.0, 0.0);
  glVertex3f (1.0, 1.0, 0.0);
  glVertex3f (1.0, 1.5, 0.0);
  glVertex3f (-1.0, 1.5, 0.0);
  glEnd ();

  // draw a triangle
  drawTriangle (1.5, -0.5, 0.0,
		1.6, 1.0, -0.2,
		1.1, 1.25, 1.0);

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

