//
// File: drawTeapot.cc
//
//  Author: Douglas Harms
//
// This program reads in the vertices, normals, and indices for the utah
// teapot and displays several teapots
//

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30;       // eye position to z-axis
static float phi=30;          // eye position to x-z plane
static float dist=5.0;

// buffer ids for the teapot
GLuint vertexBuffer, normalBuffer, indexBuffer;
int numIndices;


void setCamera(void)
{
  // sets the camera position in the world

  // set the view transformation
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -dist);
  glRotatef(phi, 1.0, 0.0, 0.0);
  glRotatef(theta, 0.0, 1.0, 0.0);
}

// read in the definitions of the teapot and store this information
// in the gpu
void defineTeapot( const char* vertexFilename,
		   const char* normalFilename,
		   const char* indexFilename )
{
  
  ifstream vertFile, normalFile, indexFile;
  GLfloat val;
  
  // read in the vertices
  vector<GLfloat> vertices;

  // open the vertex file, and return if an error occurs
  vertFile.open( vertexFilename );
  if( !vertFile ) {
    cout << "error opening " << vertexFilename << endl;
    return;
  }

  // read in all vertices
  while( vertFile >> val )
    vertices.push_back( val );

  // close the file
  vertFile.close();

  // store the vertices in the gpu
  glGenBuffers( 1, &vertexBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER,
		sizeof(GLfloat)*vertices.size(), // number of bytes in vector
		vertices.data(), // pointer to the data in vector
		GL_STATIC_DRAW );

  // now read in the normals
  vector<GLfloat> normals;

  // open the normals file, and return if an error occurs
  normalFile.open( normalFilename );
  if( !normalFile ) {
    cout << "error opening " << normalFilename << endl;
    return;
  }

  // read in all normals
  while( normalFile >> val )
    normals.push_back( val );

  // close the file
  normalFile.close();

  // save the normals in the GPU
  
  glGenBuffers( 1, &normalBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
  glBufferData( GL_ARRAY_BUFFER,
		sizeof(GLfloat) * normals.size(),
		normals.data(),
		GL_STATIC_DRAW );
 
  // read in the triangle indices 
  vector<GLuint> indices;

  // open the index file, and return if an error occurs
  indexFile.open( indexFilename );
  if( !indexFile ) {
    cout << "error opening " << indexFilename << endl;
    return;
  }

  // read in indices, keeping track of how many
  GLuint index;
  while( indexFile >> index ) {
    indices.push_back( index );
    numIndices++;
  }

  // close the file
  indexFile.close();
  
  // store indices in GPU
  glGenBuffers( 1, &indexBuffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * indices.size(),
		indices.data(),
		GL_STATIC_DRAW );
}

void displayTeapot()
{
  // enable vertices
  glEnableClientState( GL_VERTEX_ARRAY );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glVertexPointer( 3, GL_FLOAT, 0, 0 );

  // enable normals
  glEnableClientState( GL_NORMAL_ARRAY );
  glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
  glNormalPointer( GL_FLOAT, 0, 0);

  // bind the index array
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

  // now draw the teapot
  glDrawElements( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0 );

  // disable things that we enabled
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );

  // no buffers selected now
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
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
  
  // define clear color to be white
  glClearColor( 1.0, 1.0, 1.0, 1.0 );

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

  // create teapot
  defineTeapot( "TeapotVertices.txt", "TeapotNormals.txt", "TeapotIndices.txt" );
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

  // draw some cylinders
  glColor3f( 1.0, 0.0, 0.0 );
  displayTeapot();

  glPushMatrix();
  glRotatef( -90.0, 1, 0, 0 );
  glTranslatef( 1.5, 1.0, -0.5 );
  glScalef( 0.5, 0.5, 0.5 );
  glColor3f( 0.0, 0.0, 1.0 );
  displayTeapot();
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
  }
}

// onExit will be invoked when the user quits the program
void onExit( void ) 
{
  // clear buffers from gpu
  glDeleteBuffers( 1, &vertexBuffer );
  glDeleteBuffers( 1, &normalBuffer );
  glDeleteBuffers( 1, &indexBuffer );
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

