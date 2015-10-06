//
// File: ripple.cc
//
//  Author: Douglas Harms
//
// This program creates a mesh that ripples.  The rippling is done in the
// vertex shader.
//

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderUtils.h"

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30 * M_PI / 180.0;       // eye position to z-axis
static float phi=30 * M_PI / 180.0;          // eye position to x-z plane
static float dist=5.0;

// program state information
bool animate;
GLfloat rippleTime;

// current projection and modelview transformations
glm::mat4 projection, ctm;

// buffer ids for the mesh
GLuint vertexBuffer, indexBuffer;
int numIndices;

// shader uniform locations
GLint ctmLocation, timeLocation;

// vertex shader attribute locations
GLint vPositionLocation;

// glsl shader program
GLuint shaderProgram;

void setCamera(void)
{
  // sets the camera position in the world

  //ctm = glm::mat4( 1.0 );
  ctm = glm::translate( projection, glm::vec3( 0, 0, -dist ) );
  ctm = glm::rotate( ctm, phi, glm::vec3( 1, 0, 0 ) );
  ctm = glm::rotate( ctm, theta, glm::vec3( 0, 1, 0 ) );
}

// define the mesh.  All vertices are in the xz plane.
void defineMesh()
{
  // dimensions of the mesh in world coordinates
  const float WIDTH_X = 4.0;
  const float WIDTH_Z = 4.0;

  // number of vertices in each direction
  const int NUM_X = 40;
  const int NUM_Z = 40;

  // define vertices
  glm::vec3 vertices[ NUM_X * NUM_Z ];
  int pos = 0;
  for( int x=0; x<NUM_X; x++ )
    for( int z=0; z<NUM_Z; z++ )
      vertices[pos++] = glm::vec3( x*WIDTH_X/(NUM_X-1) - WIDTH_X/2.0,
				   0.0,
				   z*WIDTH_Z/(NUM_Z-1) - WIDTH_Z/2.0
				   );

  // define the indices
  GLuint indices[ 6 * (NUM_X-1) * (NUM_Z-1) ];
  pos = 0;
  for( int x=0; x<NUM_X-1; x++ )
    for( int z=0; z<NUM_Z-1; z++ ) {
      // calculate the indices of the quad
      int i0 = x * NUM_X + z;  // top left
      int i1 = i0 + 1; // bottom left
      int i2 = (x+1) * NUM_X + z;  //top right
      int i3 =  i2 + 1; // bottom right

      // define the two triangles
      indices[pos++] = i0;
      indices[pos++] = i1;
      indices[pos++] = i3;
      indices[pos++] = i0;
      indices[pos++] = i3;
      indices[pos++] = i2;
    }

  // store vertices in GPU
  glGenBuffers( 1, &vertexBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // store triangle indices in GPU
  glGenBuffers( 1, &indexBuffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
		GL_STATIC_DRAW );

  // remember the number of indices
  numIndices = 6 * (NUM_X-1) * (NUM_Z-1);
}

void displayMesh()
{
  // enable vertices
  glEnableVertexAttribArray( vPositionLocation );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glVertexAttribPointer( vPositionLocation, // which attribute
			 3, // number of elements 
			 GL_FLOAT, // element type
			 GL_FALSE, // do not convert ints to [-1,1]
			 0, // distance between element
			 0 // use the current GL_ARRAY_BUFFER
			 );

  // bind the index array
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

  // now draw the mesh
  glDrawElements( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0 );

  // disable things that we enabled
  glDisableVertexAttribArray( vPositionLocation );

  // no buffers selected now
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void init(void)
{
  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // create shader program and use it for this openGL program
  shaderProgram = initShaders( "vShader.glsl", "fShader.glsl" );
  glUseProgram( shaderProgram );

  // get locations of attributes and uniforms
  ctmLocation = glGetUniformLocation( shaderProgram, "ctm" );
  timeLocation = glGetUniformLocation( shaderProgram, "time" );
  vPositionLocation = glGetAttribLocation( shaderProgram, "vPosition" );

  // shade polygons as flat surfaces
  glShadeModel(GL_SMOOTH);

  // do hidden surface removal
  glEnable (GL_DEPTH_TEST);

  // create color cube and store in GPU
  defineMesh();

  animate = false;
  rippleTime = 0.0;
}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the camera
  setCamera();

  // send ctm and rippleTime to gpu
  glUniformMatrix4fv( ctmLocation, 1, GL_FALSE, glm::value_ptr(ctm) ); 
  glUniform1f( timeLocation, rippleTime ); 

  // draw the mesh as lines
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  displayMesh();

  // display things  
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  GLfloat aspect =  (GLfloat)w/(GLfloat)h;
  GLfloat fov = 60.0f * M_PI/180.0f;
  projection = glm::perspective(fov, aspect, 0.01f, 20.0f);
}

void timer( int val )
{
  if( !animate )
    return;

  glutTimerFunc( 100, timer, 0 );

  rippleTime += 0.5;

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
    theta += 5 * M_PI / 180.0;
    glutPostRedisplay();
    break;
  case '6': //right
    theta -= 5 * M_PI / 180.0;
    glutPostRedisplay();
    break;
  case '2': //down
    phi -= 5 * M_PI / 180.0;
    // don't let elevation go past 90 or -90
    if (phi > M_PI/2.0)
      phi = M_PI/2.0;
    else if (phi < -M_PI/2.0)
      phi = -M_PI/2.0;
    glutPostRedisplay();
    break;
  case '8': //up
    phi += 5 * M_PI / 180.0;
    // don't let elevation go past 90 or -90
    if (phi > M_PI/2.0)
      phi = M_PI/2.0;
    else if (phi < -M_PI/2.0)
      phi = -M_PI/2.0;
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

  case 'A':
  case 'a':
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
  glDeleteBuffers( 1, &vertexBuffer );
  glDeleteBuffers( 1, &indexBuffer );
}

int main(int argc, char** argv)
{
  // initialize glut, create window, etc.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Ripples!!");

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

