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

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30;       // eye position to z-axis
static float phi=30;          // eye position to x-z plane
static float dist=5.0;

// buffer ids for the color cube
GLuint vertexBuffer, colorBuffer, indexBuffer;
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
  glEnableClientState( GL_VERTEX_ARRAY );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glVertexPointer( 3, GL_FLOAT, 0, 0 );

  // enable vertex colors
  glEnableClientState( GL_COLOR_ARRAY );
  glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );
  glColorPointer( 3, GL_FLOAT, 0, 0 );

  // bind the index array
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

  // now draw the color cube
  glDrawElements( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0 );

  // disable things that we enabled
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );

  // no buffers selected now
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void init(void)
{
  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // shade polygons as flat surfaces
  glShadeModel(GL_SMOOTH);

  // do hidden surface removal
  glEnable (GL_DEPTH_TEST);

  // create color cube and store in GPU
  defineColorCube();
}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set viewpoint
  glLoadIdentity();

  // set the camera
  setCamera();

  // draw the color cube
  displayColorCube();

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

    // TODO - implement 'c', 'C', 's', 'S', '+', and '-' as described in
    // the assignment

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

