//
// File: hw6.cc
//
//  Author: 
//
// This program defines the vertices, normals, and triangles for several
// geometric shapes, stores these in GPU buffers, then draws several shapes
// in the world.
//

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30;       // eye position to z-axis
static float phi=30;          // eye position to x-z plane
static float dist=5.0;

// buffer ids for the cylinder
GLuint sphereVertexBuffer, sphereNormalBuffer, sphereIndexBuffer;
int sphereNumIndices;
GLuint pyramidVertexBuffer, pyramidColorBuffer, pyramidIndexBuffer;
int pyramidNumIndices;

// some global state information
bool animate;
float pyramidTheta;

void setCamera(void)
{
  // sets the camera position in the world

  // set the view transformation
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -dist);
  glRotatef(phi, 1.0, 0.0, 0.0);
  glRotatef(theta, 0.0, 1.0, 0.0);
}

// define the vertices, normals, and triangles for a unit sphere centered
// around the origin, and store these in GPU buffers.  Global variables
// sphereVertexBuffer, sphereNormalBuffer, sphereIndexBuffer, and 
// sphereNumIndices will be set appropriately.
void defineUnitSphere( int numSides, int numStacks )
{
}

// display the sphere whose attributes have been stored in GPU buffers
// sphereVertexBuffer, sphereNormalBuffer, and sphereIndexBuffer, with 
// sphereNumIndices indices.
void displaySphere()
{
}

// define the vertices, normals, and triangles for a pyramid centered
// around the origin, and store these in GPU buffers.  Global variables
// pyramidVertexBuffer, pyramidColorBuffer, pyramidIndexBuffer, and 
// pyramidNumIndices will be set appropriately.
void definePyramid( )
{
}

// display the pyramid whose attributes have been stored in GPU buffers
// pyramidVertexBuffer, pyramidColorBuffer, and pyramidIndexBuffer, with 
// pyramidNumIndices indices.
void displayPyramid()
{
}

void init(void)
{
  // define material property parameters
  GLfloat matSpecular[]  = {1.0, 1.0, 1.0, 1.0};
  GLfloat matShininess =   30.0;

  // define light property parameters
  GLfloat lightAmbient0[] = {0.0, 0.0, 0.2, 1.0};
  GLfloat lightDiffuse0[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightSpecular0[]= {1.0, 1.0, 1.0, 1.0};
  
  GLfloat lightAmbient1[] = {0.0, 0.0, 0.2, 1.0};
  GLfloat lightDiffuse1[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightSpecular1[]= {1.0, 1.0, 1.0, 1.0};
  
  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // set up ambient, diffuse, and specular components for the lights
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
  
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);

  // We'll use glColor to set the diffuse and ambient material properties
  glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable (GL_COLOR_MATERIAL);

  // define default material properties
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

  // two-sided
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, 1 );

  // let openGL calculate unit vector normals
  glEnable( GL_NORMALIZE );

  // lights 0 and 1 are enables
  glEnable( GL_LIGHT0 );
  glEnable( GL_LIGHT1 );

  // shade polygons smoothly
  glShadeModel(GL_SMOOTH);

  // do hidden surface removal
  glEnable (GL_DEPTH_TEST);

  // create the shapes and store attributes in teh GPU
  defineUnitSphere( 50, 50 );
  definePyramid();

  // define initial global state
  animate = false;
  pyramidTheta = 0.0;
}

void display(void)
{
  // define light positions
  GLfloat light0Pos[] = { 0.0, 2.0, 0.0, 1.0 };
  GLfloat light1Pos[] = { 0.5, 1.0, 0.0, 1.0 };

  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set viewpoint
  glLoadIdentity();

  // light1 is in camera frame
  glLightfv( GL_LIGHT1, GL_POSITION, light1Pos );

  // set the camera
  setCamera();

  // light0 is in world frame
  glLightfv( GL_LIGHT0, GL_POSITION, light1Pos );

  // let's draw the axes first
  // axes won't be lighted
  glDisable( GL_LIGHTING );

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

  // other objects in the world will be lighted
  glEnable( GL_LIGHTING );

  // draw some spheres and pyramids
  glColor3f( 1.0, 0.0, 0.0 );
  displaySphere();

  glPushMatrix();
  glRotatef( -90.0, 1, 0, 0 );
  glTranslatef( 1.5, 1.0, -0.5 );
  glScalef( 0.5, 0.5, 1.0 );
  glColor3f( 0.0, 0.0, 1.0 );
  displaySphere();
  glPopMatrix();

  // Normals weren't defined for the pyramid so turn lighting off
  glDisable( GL_LIGHTING );
  glPushMatrix();
  glRotatef( pyramidTheta, 0, 1, 0 );
  glTranslatef( 2.0, 0, 0 );
  glRotatef( pyramidTheta*10, 0, 1, 0 );
  displayPyramid();
  glPopMatrix();

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

  pyramidTheta += 2.0;
  glutPostRedisplay();
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

  case 'a':
  case 'A':
    animate = !animate;
    if( animate )
      glutTimerFunc( 100, timer, 0 );
    break;
  }
}

// onExit will be invoked when the user quits the program
void onExit( void ) 
{
  // clear buffers from gpu
  glDeleteBuffers( 1, &sphereVertexBuffer );
  glDeleteBuffers( 1, &sphereNormalBuffer );
  glDeleteBuffers( 1, &sphereIndexBuffer );
  glDeleteBuffers( 1, &pyramidVertexBuffer );
  glDeleteBuffers( 1, &pyramidColorBuffer );
  glDeleteBuffers( 1, &pyramidIndexBuffer );

}

int main(int argc, char** argv)
{
  // initialize glut, create window, etc.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Color Cube!");

  // initialize glew
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    cout << "Error: " << glewGetErrorString(glew_status) << endl;
    return 1;
  }

  // initialize program data structures
  init();

  // register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // callback to clean up things on exit
  atexit( onExit );

  // do everything!
  glutMainLoop();

  return 0;

}

