//
// File: colorCube.cc
//
//  Author: Douglas Harms
//
// This program creates a color cube and displays it in the world.  This program
// uses vertex arrays stored in the GPU.
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
float cube1Theta, cube1Phi, cube2Theta, cube2Phi;
bool animate;

// current projection and modelview transformations
glm::mat4 projection, ctm;

// buffer ids for the color cube
GLuint vertexBuffer, colorBuffer, indexBuffer;
int numIndices;
GLuint colorCubeArray;

// glsl shader program
GLuint shaderProgram;

void setCamera(void)
{
  // sets the camera position in the world

  //ctm = glm::mat4( 1.0 );
  ctm = glm::translate( projection, glm::vec3( 0, 0, -dist ) );
  ctm = glm::rotate( ctm, phi, glm::vec3( 1, 0, 0 ) );
  ctm = glm::rotate( ctm, theta, glm::vec3( 0, 1, 0 ) );

  // set the view transformation
  //glLoadIdentity();
  //glTranslatef(0.0, 0.0, -dist);
  //glRotatef(phi, 1.0, 0.0, 0.0);
  //glRotatef(theta, 0.0, 1.0, 0.0);
}

// define the vertices, colors, and triangles for the color cube
void defineColorCube()
{
  GLfloat vertices[][3] = {
    {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
    {1, -1, -1}, {1, -1, 1}, {1, 1, -1}, {1, 1, 1}
  };
  GLfloat colors[][3] = {
    {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
    {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
  };
  GLuint indices[][3] = {
    {0,1,3}, {0,3,2}, // left
    {1,5,7}, {1,7,3}, // front
    {5,4,6}, {5,6,7}, // right
    {4,0,2}, {4,2,6}, // back
    {2,3,7}, {2,7,6}, // top
    {0,4,5}, {0,5,1}  // bottom
  };
    
  // store vertices in GPU
  glGenBuffers( 1, &vertexBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // store vertex colors in GPU
  glGenBuffers( 1, &colorBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  // store triangle indices in GPU
  glGenBuffers( 1, &indexBuffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
		GL_STATIC_DRAW );

  // there are 36 indices in the array
  numIndices = 36;
}

void displayColorCube()
{
  // enable vertices
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glVertexAttribPointer( vPosition, // which attribute
			 3, // number of elements 
			 GL_FLOAT, // element type
			 GL_FALSE, // do not convert ints to [-1,1]
			 0, // distance between element
			 0 // use the current GL_ARRAY_BUFFER
			 );

  // enable vertex colors
  GLint vColor = glGetAttribLocation( shaderProgram, "vColor" );
  glEnableVertexAttribArray( vColor );
  glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );
  glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // bind the index array
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

  // now draw the color cube
  glDrawElements( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0 );

  // disable things that we enabled
  glDisableVertexAttribArray( vPosition );
  glDisableVertexAttribArray( vColor );

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

  // shade polygons as flat surfaces
  glShadeModel(GL_SMOOTH);

  // do hidden surface removal
  glEnable (GL_DEPTH_TEST);

  // create color cube and store in GPU
  defineColorCube();

  // initialize global state
  cube1Theta = 0.0;
  cube1Phi = 0.0;
  cube2Theta = 0.0;
  cube2Phi = 0.0;

  animate = false;
}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set viewpoint
  //glLoadIdentity();

  // set the camera
  setCamera();

  // draw things in the world

  // send the ctm to the vertex shader
  GLuint ctmLocation = glGetUniformLocation( shaderProgram, "ctm" );
  glUniformMatrix4fv( ctmLocation, 1, GL_FALSE, glm::value_ptr(ctm) ); 

  // now draw the color cube
  displayColorCube();

  // remember current ctm for later retrieval
  glm::mat4 worldFrame = ctm;

  // transform to cube1 frame and display the cube
  ctm = glm::rotate( ctm, cube1Theta, glm::vec3( 0, 1, 0 ) );
  ctm = glm::translate( ctm, glm::vec3( 1.75, 0, 0 ) );
  ctm = glm::rotate( ctm, cube1Phi, glm::vec3( 0, 1, 0 ) );
  ctm = glm::scale( ctm, glm::vec3( 0.3, 0.3, 0.3 ) );
  glUniformMatrix4fv( ctmLocation, 1, GL_FALSE, glm::value_ptr(ctm) ); 
  displayColorCube();

  // set ctm back to worldframe
  ctm = worldFrame;

  // now transform to cube2 frame and display the cube
  ctm = glm::rotate( ctm, cube2Theta, glm::vec3( 1, 0, 0 ) );
  ctm = glm::translate( ctm, glm::vec3( 0, 1.75, 0 ) );
  ctm = glm::rotate( ctm, cube2Phi, glm::vec3( 0, 1, 0 ) );
  ctm = glm::scale( ctm, glm::vec3( 0.3, 0.3, 0.3 ) );
  glUniformMatrix4fv( ctmLocation, 1, GL_FALSE, glm::value_ptr(ctm) ); 
  displayColorCube();

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

  cube1Theta += 5 * M_PI / 180.0;
  cube1Phi -= 15 * M_PI / 180.0;
  cube2Theta -= 7 * M_PI / 180.0;
  cube2Phi += 10 * M_PI / 180.0;

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
  glDeleteBuffers( 1, &colorBuffer );
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

